#include "Areas.h"
#include "Log.h"

EU4::Areas::Areas(std::istream& theStream)
{
	registerRegex("[\\w_]+", [this](const std::string& areaName, std::istream& areasFile)
		{
			Area newArea(areasFile);
			theAreas.insert(std::make_pair(areaName, newArea.getProvinces()));
		});

	parseStream(theStream);
}

const std::set<int> EU4::Areas::getProvincesInArea(const std::string& area) const
{
	auto areaItr(theAreas.find(area));
	if (areaItr != theAreas.end())
	{
		return areaItr->second;
	}
	else
	{
		return {};
	}
}