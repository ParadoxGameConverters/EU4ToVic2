#include "Building.h"
#include "ParserHelpers.h"

EU4::Building::Building(std::istream& theStream)
{
	registerKeyword(std::regex("cost"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleDouble costDouble(theStream);
		cost = costDouble.getDouble();
	});
	registerKeyword(std::regex("modifier"), [this](const std::string& unused, std::istream& theStream) {
		modifier = Modifier(theStream);
	});
	registerKeyword(std::regex("manufactory"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::ignoreItem(unused, theStream);
		manufactory = true;
	});
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}