#include "ProvinceHistory.h"
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

	registerKeyword("owner", [this](const std::string& unused, std::istream & theStream) {
		commonItems::singleString ownerString(theStream);
		ownershipHistory.push_back(std::make_pair(STARTING_DATE, ownerString.getString()));
	});
	registerKeyword("culture", [&startingCulture](const std::string& unused, std::istream & theStream) {
		commonItems::singleString cultureString(theStream);
		startingCulture = cultureString.getString();
	});
	registerKeyword("religion", [&startingReligion](const std::string& unused, std::istream & theStream) {
		commonItems::singleString religionString(theStream);
		startingReligion = religionString.getString();
	});
	registerKeyword("base_tax", [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleDouble baseTaxDouble(theStream);
		originalTax = baseTaxDouble.getDouble();
	});
	registerKeyword("base_production", [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleDouble baseProductionDouble(theStream);
		originalProduction = baseProductionDouble.getDouble();
	});
	registerKeyword("base_manpower", [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleDouble manpowerDouble(theStream);
		originalManpower = manpowerDouble.getDouble();
	});
	registerRegex("\\d+\\.\\d+\\.\\d+", [this](const std::string& dateString, std::istream& theStream) {
		date theDate = date(dateString);
		DateItems theItems(theStream);
		for (const auto& theItem : theItems.getDateChanges())
		{
			switch (theItem.first) {
			case DateItemType::OWNER_CHANGE:
				ownershipHistory.push_back(std::make_pair(theDate, theItem.second));
				break;
			case DateItemType::CULTURE_CHANGE:
				cultureHistory.push_back(std::make_pair(theDate, theItem.second));
				break;
			case DateItemType::RELIGION_CHANGE:
				religionHistory.push_back(std::make_pair(theDate, theItem.second));
				break;
			}
		}
	});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();

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
	if (ownershipHistory.size() > 0) return ownershipHistory[0].first;
	return {};
}

bool EU4::ProvinceHistory::hasOriginalCulture() const
{
	if ((cultureHistory.size() > 1) && (cultureHistory[0].second != cultureHistory[cultureHistory.size() - 1].second)) return false;
	return true;
}

bool EU4::ProvinceHistory::wasColonized() const
{
	if ((ownershipHistory.size() > 0) &&
		(ownershipHistory[0].first != STARTING_DATE) &&
		(ownershipHistory[0].first != theConfiguration.getFirstEU4Date())) 
	{
		return !hasOriginalCulture();
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
	if (oldDate == STARTING_DATE) return;

	auto diffInYears = newDate.diffInYears(oldDate);
	for (auto& popRatio: popRatios) popRatio.decay(diffInYears, currentPop);

	currentPop.increase(diffInYears);
}