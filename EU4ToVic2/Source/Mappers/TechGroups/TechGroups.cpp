#include "TechGroups.h"
#include "CommonRegexes.h"
#include "Configuration.h"
#include "ParserHelpers.h"

mappers::TechGroups::TechGroups(std::istream& theStream)
{
	registerKeyword("westernization", [this](std::istream& theStream) {
		westernization = commonItems::getInt(theStream);
	});
	registerKeyword("literacy", [this](std::istream& theStream) {
		literacyBoost = commonItems::getInt(theStream);
	});
	registerKeyword("industry", [this](std::istream& theStream) {
		industry = commonItems::getInt(theStream);
	});
	registerKeyword("cultures", [this](std::istream& theStream) {
		const auto& cultureList = commonItems::getStrings(theStream);
		cultures.insert(cultureList.begin(), cultureList.end());
	});
	registerKeyword("hpm_cultures", [this](std::istream& theStream) {
		const auto& cultureList = commonItems::getStrings(theStream);
		hpmCultures.insert(cultureList.begin(), cultureList.end());
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();

	if (theConfiguration.isHpmEnabled() && !hpmCultures.empty())
		cultures = hpmCultures;
}