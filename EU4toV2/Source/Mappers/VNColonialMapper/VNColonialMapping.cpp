#include "VNColonialMapping.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

mappers::VNColonialMapping::VNColonialMapping(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::VNColonialMapping::registerKeys()
{
	registerKeyword("name", [this](std::istream& theStream) {
		name = commonItems::getString(theStream);
	});
	registerKeyword("key", [this](std::istream& theStream) {
		keyProvince = commonItems::getInt(theStream);
	});
	registerKeyword("provinces", [this](std::istream& theStream) {
		provinces = commonItems::getInts(theStream);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
