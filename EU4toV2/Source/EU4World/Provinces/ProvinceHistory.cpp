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
#include "../../Configuration.h"
#include "Log.h"
#include "ParserHelpers.h"



const date STARTING_DATE = theConfiguration.getStartEU4Date();
const date HARD_ENDING_DATE("1836.1.1");
const date FUTURE_DATE("2000.1.1");



EU4::ProvinceHistory::ProvinceHistory(std::istream& theStream)
{
	std::string startingCulture;
	std::string startingReligion;

	registerKeyword(std::regex("owner"), [this](const std::string& unused, std::istream & theStream) {
		commonItems::singleString ownerString(theStream);
		ownershipHistory.push_back(std::make_pair(STARTING_DATE, ownerString.getString()));
	});
	registerKeyword(std::regex("culture"), [&startingCulture](const std::string& unused, std::istream & theStream) {
		commonItems::singleString cultureString(theStream);
		startingCulture = cultureString.getString();
	});
	registerKeyword(std::regex("religion"), [&startingReligion](const std::string& unused, std::istream & theStream) {
		commonItems::singleString religionString(theStream);
		startingReligion = religionString.getString();
	});
	registerKeyword(std::regex("base_tax"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleDouble baseTaxDouble(theStream);
		originalTax = baseTaxDouble.getDouble();
	});
	registerKeyword(std::regex("base_production"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleDouble baseProductionDouble(theStream);
		originalProduction = baseProductionDouble.getDouble();
	});
	registerKeyword(std::regex("base_manpower"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleDouble manpowerDouble(theStream);
		originalManpower = manpowerDouble.getDouble();
	});
	registerKeyword(std::regex("\\d+\\.\\d+\\.\\d+"), [this](const std::string& dateString, std::istream& theStream) {
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

	if ((startingCulture != "") && ((cultureHistory.size() == 0) || (cultureHistory.begin()->first != STARTING_DATE)))
	{
		cultureHistory.insert(cultureHistory.begin(), std::make_pair(STARTING_DATE, startingCulture));
	}
	if ((startingReligion != "") && ((religionHistory.size() == 0) || (religionHistory.begin()->first != STARTING_DATE)))
	{
		religionHistory.insert(religionHistory.begin(), std::make_pair(STARTING_DATE, startingReligion));
	}

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


bool EU4::ProvinceHistory::wasColonized() const
{
	if (
		(ownershipHistory.size() > 0) &&
		(ownershipHistory[0].first != STARTING_DATE) &&
		(ownershipHistory[0].first != theConfiguration.getFirstEU4Date())
	) {
		return !hasOriginalCulture();
	}
	else
	{
		return false;
	}
}


bool EU4::ProvinceHistory::wasInfidelConquest(const Religions& allReligions, const std::string& ownerReligionString, int num) const
{
	// returns true if the original religion and the current owner's religion are in different groups
	// and the province was NOT colonized
	if (religionHistory.size() > 0 && !wasColonized())
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
			if (!hasOriginalCulture())
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
	if (endDate > HARD_ENDING_DATE)
	{
		endDate = HARD_ENDING_DATE;
	}

	std::string startingCulture;
	std::vector<std::pair<date, std::string>>::iterator cultureEvent = cultureHistory.begin();
	if (cultureEvent != cultureHistory.end())
	{
		startingCulture = cultureEvent->second;
		++cultureEvent;
	}

	std::string startingReligion;
	std::vector<std::pair<date, std::string>>::iterator religionEvent = religionHistory.begin();
	if (religionEvent != religionHistory.end())
	{
		startingReligion = religionEvent->second;
		++religionEvent;
	}

	EU4::PopRatio currentRatio(startingCulture, startingReligion);
	date cultureEventDate;
	date religionEventDate;
	date lastLoopDate = STARTING_DATE;
	while (cultureEvent != cultureHistory.end() || religionEvent != religionHistory.end())
	{
		if (cultureEvent == cultureHistory.end())
		{
			cultureEventDate = FUTURE_DATE;
		}
		else
		{
			cultureEventDate = cultureEvent->first;
		}

		if (religionEvent == religionHistory.end())
		{
			religionEventDate = FUTURE_DATE;
		}
		else
		{
			religionEventDate = religionEvent->first;
		}

		if (cultureEventDate < religionEventDate)
		{
			decayPopRatios(lastLoopDate, cultureEventDate, currentRatio);
			popRatios.push_back(currentRatio);
			for (auto& itr: popRatios)
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
			for (auto& itr: popRatios)
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
			for (auto& itr: popRatios)
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
	if (oldDate == STARTING_DATE)
	{
		return;
	}

	auto diffInYears = newDate.diffInYears(oldDate);
	for (auto& popRatio: popRatios)
	{
		popRatio.decay(diffInYears, currentPop);
	}

	currentPop.increase(diffInYears);
}