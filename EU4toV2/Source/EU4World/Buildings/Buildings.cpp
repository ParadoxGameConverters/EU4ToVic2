#include "Buildings.h"
#include "ParserHelpers.h"

EU4::Buildings::Buildings(std::istream& theStream)
{
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this](const std::string& buildingName, std::istream& theStream) {
		Building building(theStream);
		buildings.insert(std::make_pair(buildingName, building));
	});
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}

std::optional<EU4::Building> EU4::Buildings::getBuilding(const std::string& buildingName) const
{
	if (buildings.count(buildingName) > 0)
	{
		return std::make_optional(buildings.at(buildingName));
	}
	else
	{
		return std::nullopt;
	}
}