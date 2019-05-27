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
#include "DateItem.h"
#include "DateItems.h"
#include "../Configuration.h"
#include "Log.h"
#include "ParserHelpers.h"



EU4::ProvinceHistory::ProvinceHistory(std::istream& theStream)
{
	std::string lastOwner;

	registerKeyword(std::regex("owner"), [this, &lastOwner](const std::string& unused, std::istream & theStream) {
		commonItems::singleString ownerString(theStream);
		lastOwner = ownerString.getString();
		ownershipHistory.push_back(std::make_pair(date("1444.11.11"), lastOwner));
	});
	registerKeyword(std::regex("culture"), [this](const std::string& unused, std::istream & theStream) {
		commonItems::singleString cultureString(theStream);
		cultureHistory.push_back(std::make_pair(date("1444.11.11"), cultureString.getString()));
	});
	registerKeyword(std::regex("religion"), [this](const std::string& unused, std::istream & theStream) {
		commonItems::singleString religionString(theStream);
		religionHistory.push_back(std::make_pair(date("1444.11.11"), religionString.getString()));
	});
	registerKeyword(std::regex("\\d+\\.\\d+\\.\\d+"), [this, &lastOwner](const std::string& dateString, std::istream& theStream) {
		DateItems theItems(dateString, theStream);
		for (DateItem item : theItems.getItems())
		{
			if (item.getType() == DateItemType::OWNER_CHANGE)
			{
				ownershipHistory.push_back(std::make_pair(item.getDate(), item.getData()));

			}
			else if (item.getType() == DateItemType::CULTURE_CHANGE)
			{
				cultureHistory.push_back(std::make_pair(item.getDate(), item.getData()));
			}
			else if (item.getType() == DateItemType::RELIGION_CHANGE)
			{
				religionHistory.push_back(std::make_pair(item.getDate(), item.getData()));
			}
		}
	});
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), commonItems::ignoreItem);

	parseStream(theStream);

	sort(ownershipHistory.begin(), ownershipHistory.end());
	sort(cultureHistory.begin(), cultureHistory.end());
	sort(religionHistory.begin(), religionHistory.end());

	buildPopRatios();
}


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


bool EU4::ProvinceHistory::hasOriginalCulture() const
{
	if ((cultureHistory.size() > 1) && (cultureHistory[0].second != cultureHistory[cultureHistory.size() - 1].second))
	{
		return false;
	}
	else
	{
		return true;
	}
}


bool EU4::ProvinceHistory::wasInfidelConquest(const Religions& allReligions, const std::string& ownerReligionString, bool wasColonized, int num) const
{
	// returns true if the province was originally pagan, the current owner is non-pagan,
	// and the province was NOT colonized
	if (religionHistory.size() > 0 && !wasColonized)
	{
		std::optional<Religion> originalReligion = allReligions.getReligion(religionHistory[0].second);
		std::optional<Religion> ownerReligion = allReligions.getReligion(ownerReligionString);
		if (!originalReligion || !ownerReligion)
		{
			LOG(LogLevel::Warning) << "Unhandled religion in EU4 province " << num;
			return true;
		}
		else
		{
			if ((cultureHistory.size() > 1) && (cultureHistory[0].second != cultureHistory[cultureHistory.size() - 1].second))
			{
				return originalReligion->isInfidelTo(*ownerReligion);
			}
		}
	}
	return false;
}


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