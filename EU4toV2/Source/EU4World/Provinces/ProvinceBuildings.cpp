#include "ProvinceBuildings.h"
#include "ParserHelpers.h"

EU4::ProvinceBuildings::ProvinceBuildings(std::istream& theStream)
{
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this](const std::string& building, std::istream& theStream) {
		commonItems::ignoreItem(building, theStream);
		buildings.insert(building);
	});
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}