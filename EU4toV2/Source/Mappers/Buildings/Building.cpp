#include "Building.h"
#include "ParserHelpers.h"

mappers::Building::Building(std::istream& theStream)
{
	registerKeyword("cost", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleDouble costDouble(theStream);
		cost = costDouble.getDouble();
	});
	registerKeyword("modifier", [this](const std::string& unused, std::istream& theStream) {
		modifier = EU4::Modifier(theStream);
	});
	registerKeyword("manufactory", [this](const std::string& unused, std::istream& theStream) {
		commonItems::ignoreItem(unused, theStream);
		manufactory = true;
	});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}