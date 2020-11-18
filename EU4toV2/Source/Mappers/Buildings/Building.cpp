#include "Building.h"
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
	registerKeyword("modifier", [this](const std::string& unused, std::istream& theStream) {
		modifier = EU4::Modifier(theStream);
	});
	registerKeyword("manufactory", [this](const std::string& unused, std::istream& theStream) {
		commonItems::ignoreItem(unused, theStream);
		manufactory = true;
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
