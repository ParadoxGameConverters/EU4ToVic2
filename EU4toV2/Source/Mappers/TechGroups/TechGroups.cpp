#include "TechGroups.h"
#include "ParserHelpers.h"
#include "CommonRegexes.h"

mappers::TechGroups::TechGroups(std::istream& theStream)
{
	registerKeyword("westernization", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt westernizationInt(theStream);
		westernization = westernizationInt.getInt();
	});
	registerKeyword("literacy", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt literacyInt(theStream);
		literacyBoost = literacyInt.getInt();
	});
	registerKeyword("cultures", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::stringList culList(theStream);
		const auto& cultureList = culList.getStrings();
		cultures.insert(cultureList.begin(), cultureList.end());
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}