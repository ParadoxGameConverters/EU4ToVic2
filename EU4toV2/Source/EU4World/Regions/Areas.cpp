#include "Areas.h"
#include "Area.h"

EU4::Areas::Areas(std::istream& theStream)
{
	registerRegex("[\\w_]+", [this](const std::string& areaName, std::istream& theStream)
		{
			const Area newArea(theStream);
			theAreas.insert(std::make_pair(areaName, newArea.getProvinces()));
		});

	parseStream(theStream);
	clearRegisteredKeywords();
}

std::optional<std::set<int>> EU4::Areas::getProvincesInArea(const std::string& area) const
{
	const auto& areaItr(theAreas.find(area));
	if (areaItr != theAreas.end()) return areaItr->second;
	return std::nullopt;
}