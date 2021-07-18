#include "ProvinceMapping.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

mappers::ProvinceMapping::ProvinceMapping(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::ProvinceMapping::registerKeys()
{
	registerKeyword("eu4", [this](const std::string& unused, std::istream& theStream) {
		eu4Provinces.insert(commonItems::singleInt(theStream).getInt());
	});
	registerKeyword("v2", [this](const std::string& unused, std::istream& theStream) {
		vic2Provinces.insert(commonItems::singleInt(theStream).getInt());
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
