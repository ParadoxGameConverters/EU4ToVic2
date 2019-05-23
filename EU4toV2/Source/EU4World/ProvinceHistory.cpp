/*Copyright(c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */



#include "ProvinceHistory.h"
#include "EU4Religion.h"
#include "../Configuration.h"
#include "Log.h"



EU4::ProvinceHistory::ProvinceHistory(
	std::vector<std::shared_ptr<Object>> historyObj,
	std::vector<std::shared_ptr<Object>> culObj,
	std::vector<std::shared_ptr<Object>> religObj
)
{
	vector<shared_ptr<Object>> historyObjs = historyObj[0]->getLeaves();
	string lastOwner;				// the last owner of the province
	string thisCountry;			// the current owner of the province
	for (unsigned int i = 0; i < historyObjs.size(); i++)
	{
		if (historyObjs[i]->getKey() == "owner")
		{
			thisCountry = historyObjs[i]->getLeaf();
			lastOwner = thisCountry;
			ownershipHistory.push_back(make_pair(date(), thisCountry));
			continue;
		}
		else if (historyObjs[i]->getKey() == "culture")
		{
			cultureHistory.push_back(make_pair(date(), historyObjs[i]->getLeaf()));
			continue;
		}
		else if (historyObjs[i]->getKey() == "religion")
		{
			religionHistory.push_back(make_pair(date(), historyObjs[i]->getLeaf()));
			continue;
		}

		vector<shared_ptr<Object>> ownerObj = historyObjs[i]->getValue("owner");	// the object holding the current historical owner change
		if (ownerObj.size() > 0)
		{
			const date newDate(historyObjs[i]->getKey());	// the date this happened
			thisCountry = ownerObj[0]->getLeaf();

			map<string, date>::iterator itr = lastPossessedDate.find(lastOwner);
			if (itr != lastPossessedDate.end())
				itr->second = newDate;
			else
				lastPossessedDate.insert(make_pair(lastOwner, newDate));
			lastOwner = thisCountry;

			ownershipHistory.push_back(make_pair(newDate, thisCountry));
		}
		vector<shared_ptr<Object>> culObj = historyObjs[i]->getValue("culture");	// the object holding the current historical culture change
		if (culObj.size() > 0)
		{
			const date newDate(historyObjs[i]->getKey());	// the date this happened
			cultureHistory.push_back(make_pair(newDate, culObj[0]->getLeaf()));
		}
		vector<shared_ptr<Object>> religObj = historyObjs[i]->getValue("religion");	// the object holding the current historical religion change
		if (religObj.size() > 0)
		{
			const date newDate(historyObjs[i]->getKey());	// the date this happened
			religionHistory.push_back(make_pair(newDate, religObj[0]->getLeaf()));
		}
	}
	sort(ownershipHistory.begin(), ownershipHistory.end());
	sort(cultureHistory.begin(), cultureHistory.end());
	sort(religionHistory.begin(), religionHistory.end());

	if (cultureHistory.size() == 0)
	{
		if (culObj.size() > 0)
		{
			const date newDate;	// the default date
			cultureHistory.push_back(make_pair(newDate, culObj[0]->getLeaf()));
		}
	}
	if (religionHistory.size() == 0)
	{
		if (religObj.size() > 0)
		{
			const date newDate;	// the default date
			religionHistory.push_back(make_pair(newDate, religObj[0]->getLeaf()));
		}
	}

	buildPopRatios();
}

#pragma optimize("",off)
std::optional<date> EU4::ProvinceHistory::getFirstOwnedDate() const
{
	if (ownershipHistory.size() > 0)
	{
		return ownershipHistory[0].first;
	}
	else
	{
		return {};
	}
}


bool EU4::ProvinceHistory::ownedByOriginalOwner() const
{
	if ((cultureHistory.size() > 1) && (cultureHistory[0].second != cultureHistory[cultureHistory.size() - 1].second))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool EU4::ProvinceHistory::wasInfidelConquest(const std::string& ownerReligionString, bool wasColonized, int num) const
{
	// returns true if the province was originally pagan, the current owner is non-pagan,
	// and the province was NOT colonized
	if (religionHistory.size() > 0 && !wasColonized)
	{
		EU4Religion* firstReligion = EU4Religion::getReligion(religionHistory[0].second);	// the first religion of this province
		EU4Religion* ownerReligion = EU4Religion::getReligion(ownerReligionString);			// the owner's religion
		if ((firstReligion == NULL) || (ownerReligion == NULL))
		{
			LOG(LogLevel::Warning) << "Unhandled religion in EU4 province " << num;
			return true;
		}
		else
		{
			if ((cultureHistory.size() > 1) && (cultureHistory[0].second != cultureHistory[cultureHistory.size() - 1].second))
			{
				return firstReligion->isInfidelTo(ownerReligion);
			}
		}
	}
	return false;
}


date EU4::ProvinceHistory::getLastPossessedDate(const std::string& tag) const
{
	map<string, date>::const_iterator itr = lastPossessedDate.find(tag);
	if (itr != lastPossessedDate.end())
	{
		return itr->second;
	}
	return date();
}
#pragma optimize("",on)

void EU4::ProvinceHistory::buildPopRatios()
{
	date endDate = theConfiguration.getLastEU4Date();
	if (endDate < date("1821.1.1"))
	{
		endDate = date("1821.1.1");
	}

	std::string startingCulture;
	std::vector<std::pair<date, std::string>>::iterator cultureEvent = cultureHistory.begin();
	if (cultureEvent != cultureHistory.end())
	{
		startingCulture = cultureEvent->second;
	}

	std::string startingReligion;
	std::vector<std::pair<date, std::string>>::iterator religionEvent = religionHistory.begin();
	if (religionEvent != religionHistory.end())
	{
		startingReligion = religionEvent->second;
	}

	EU4::PopRatio currentRatio(startingCulture, startingReligion);
	date cultureEventDate;
	date religionEventDate;
	date lastLoopDate;
	while (cultureEvent != cultureHistory.end() && religionEvent != religionHistory.end())
	{
		if (cultureEvent == cultureHistory.end())
		{
			cultureEventDate = date("2000.1.1");
		}
		else
		{
			cultureEventDate = cultureEvent->first;
		}

		if (religionEvent == religionHistory.end())
		{
			religionEventDate = date("2000.1.1");
		}
		else
		{
			religionEventDate = religionEvent->first;
		}

		if (cultureEventDate < religionEventDate)
		{
			decayPopRatios(lastLoopDate, cultureEventDate, currentRatio);
			popRatios.push_back(currentRatio);
			for (auto itr : popRatios)
			{
				itr.convertFrom();
			}
			currentRatio.convertToCulture(cultureEvent->second);
			lastLoopDate = cultureEventDate;
			++cultureEvent;
		}
		else if (cultureEventDate == religionEventDate)
		{
			// culture and religion change on the same day;
			decayPopRatios(lastLoopDate, cultureEventDate, currentRatio);
			popRatios.push_back(currentRatio);
			for (auto itr : popRatios)
			{
				itr.convertFrom();
			}
			currentRatio.convertTo(cultureEvent->second, religionEvent->second);
			lastLoopDate = cultureEventDate;
			++cultureEvent;
			++religionEvent;
		}
		else if (religionEventDate < cultureEventDate)
		{
			decayPopRatios(lastLoopDate, cultureEventDate, currentRatio);
			popRatios.push_back(currentRatio);
			for (auto itr : popRatios)
			{
				itr.convertFrom();
			}
			currentRatio.convertToReligion(religionEvent->second);
			lastLoopDate = religionEventDate;
			++religionEvent;
		}
	}
	decayPopRatios(lastLoopDate, endDate, currentRatio);

	if ((currentRatio.getCulture() != "") || (currentRatio.getReligion() != ""))
	{
		popRatios.push_back(currentRatio);
	}
}


void EU4::ProvinceHistory::decayPopRatios(const date& oldDate, const date& newDate, EU4::PopRatio& currentPop)
{
	// no decay needed for initial state
	if (oldDate == date())
	{
		return;
	}

	auto diffInYears = newDate.diffInYears(oldDate);
	for (auto popRatio : popRatios)
	{
		popRatio.decay(diffInYears, currentPop);
	}

	currentPop.increase(diffInYears);
}