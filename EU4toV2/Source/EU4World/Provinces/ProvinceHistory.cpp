#include "ProvinceHistory.h"
#include "DateItems.h"
#include "../../Configuration.h"
#include "Log.h"
#include "ParserHelpers.h"

const date HARD_ENDING_DATE("1836.1.1");
const date FUTURE_DATE("2000.1.1");

EU4::ProvinceHistory::ProvinceHistory(std::istream& theStream)
{
	std::string startingCulture;
	std::string startingReligion;

	registerKeyword("owner", [this](const std::string& unused, std::istream & theStream) {
		const commonItems::singleString ownerString(theStream);
		ownershipHistory.emplace_back(std::make_pair(theConfiguration.getStartEU4Date(), ownerString.getString()));
	});
	registerKeyword("culture", [&startingCulture](const std::string& unused, std::istream & theStream) {
		const commonItems::singleString cultureString(theStream);
		startingCulture = cultureString.getString();
	});
	registerKeyword("religion", [&startingReligion](const std::string& unused, std::istream & theStream) {
		const commonItems::singleString religionString(theStream);
		startingReligion = religionString.getString();
	});
	registerKeyword("base_tax", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleDouble baseTaxDouble(theStream);
		originalTax = baseTaxDouble.getDouble();
	});
	registerKeyword("base_production", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleDouble baseProductionDouble(theStream);
		originalProduction = baseProductionDouble.getDouble();
	});
	registerKeyword("base_manpower", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleDouble manpowerDouble(theStream);
		originalManpower = manpowerDouble.getDouble();
	});
	registerRegex("\\d+\\.\\d+\\.\\d+", [this](const std::string& dateString, std::istream& theStream) {
		auto theDate = date(dateString);
		const DateItems theItems(theStream);
		for (const auto& theItem : theItems.getDateChanges())
		{
			switch (theItem.first) {
			case DateItemType::OWNER_CHANGE:
				ownershipHistory.emplace_back(std::make_pair(theDate, theItem.second));
				break;
			case DateItemType::CULTURE_CHANGE:
				cultureHistory.emplace_back(std::make_pair(theDate, theItem.second));
				break;
			case DateItemType::RELIGION_CHANGE:
				religionHistory.emplace_back(std::make_pair(theDate, theItem.second));
				break;
			}
		}
	});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();

	if (theConfiguration.getConvertAll())
	{
		// Drop conversion history if we're converting everyone by config.
		if (!cultureHistory.empty())
		{
			auto lastCulture = cultureHistory.back();
			lastCulture.first = theConfiguration.getStartEU4Date();
			cultureHistory.clear();
			cultureHistory.emplace_back(lastCulture);
		}
		if (!religionHistory.empty())
		{
			auto lastReligion = religionHistory.back();
			lastReligion.first = theConfiguration.getStartEU4Date();
			religionHistory.clear();
			religionHistory.emplace_back(lastReligion);
		}
	}

	if (!startingCulture.empty() && (cultureHistory.empty() || cultureHistory.begin()->first != theConfiguration.getStartEU4Date()))
	{
		cultureHistory.insert(cultureHistory.begin(), std::make_pair(theConfiguration.getStartEU4Date(), startingCulture));
	}
	if (!startingReligion.empty() && (religionHistory.empty() || religionHistory.begin()->first != theConfiguration.getStartEU4Date()))
	{
		religionHistory.insert(religionHistory.begin(), std::make_pair(theConfiguration.getStartEU4Date(), startingReligion));
	}

	buildPopRatios();
}

std::optional<date> EU4::ProvinceHistory::getFirstOwnedDate() const
{
	if (!ownershipHistory.empty()) return ownershipHistory[0].first;
	return std::nullopt;
}

bool EU4::ProvinceHistory::hasOriginalCulture() const
{
	if (cultureHistory.size() > 1 && cultureHistory[0].second != cultureHistory[cultureHistory.size() - 1].second) return false;
	return true;
}

bool EU4::ProvinceHistory::wasColonized() const
{
	if (!ownershipHistory.empty() &&
		ownershipHistory[0].first != theConfiguration.getStartEU4Date() &&
		ownershipHistory[0].first != theConfiguration.getFirstEU4Date()) 
	{
		return !hasOriginalCulture();
	}
	return false;
}

void EU4::ProvinceHistory::buildPopRatios()
{
	auto endDate = theConfiguration.getLastEU4Date();
	if (endDate > HARD_ENDING_DATE) endDate = HARD_ENDING_DATE;

	std::string startingCulture;
	auto cultureEvent = cultureHistory.begin();
	if (cultureEvent != cultureHistory.end())
	{
		startingCulture = cultureEvent->second;
		++cultureEvent;
	}

	std::string startingReligion;
	auto religionEvent = religionHistory.begin();
	if (religionEvent != religionHistory.end())
	{
		startingReligion = religionEvent->second;
		++religionEvent;
	}

	PopRatio currentRatio(startingCulture, startingReligion);
	date cultureEventDate;
	date religionEventDate;
	auto lastLoopDate = theConfiguration.getStartEU4Date();
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
			decayPopRatios(lastLoopDate, religionEventDate, currentRatio);
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

	if (!currentRatio.getCulture().empty() || !currentRatio.getReligion().empty())
	{
		popRatios.push_back(currentRatio);
	}
}

void EU4::ProvinceHistory::decayPopRatios(const date& oldDate, const date& newDate, PopRatio& currentPop)
{
	// no decay needed for initial state
	if (oldDate == theConfiguration.getStartEU4Date()) return;

	const auto diffInYears = newDate.diffInYears(oldDate);
	for (auto& popRatio: popRatios) popRatio.decay(diffInYears, currentPop);

	currentPop.increase(diffInYears);
}
