#include "Building.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

mappers::Building::Building(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::Building::registerKeys()
{
	registerKeyword("cost", [this](const std::string& unused, std::istream& theStream) {
		cost = commonItems::singleDouble(theStream).getDouble();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
