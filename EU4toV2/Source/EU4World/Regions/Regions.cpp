#include "Regions.h"
#include "Areas.h"

EU4::Regions::Regions(const EU4::Areas& areas, std::istream& regionsFile)
{
	registerRegex("\\w+_region", [this, areas](const std::string & areaName, std::istream & areasFile)
		{
			EU4::Region newRegion(areasFile);
			newRegion.addProvinces(areas);
			regions.insert(make_pair(areaName, newRegion));
		});

	parseStream(regionsFile);
	clearRegisteredKeywords();

	for (const auto& area : areas.getAreas()) regions.insert(std::make_pair(area.first, EU4::Region(area.second)));
}

EU4::Regions::Regions(const EU4::Areas& areas)
{
	auto theAreas = areas.getAreas();
	std::for_each(theAreas.begin(), theAreas.end(), [this](const std::pair<std::string, std::set<int>> & theArea)
	{
		regions.insert(make_pair(theArea.first, EU4::Region(theArea.second)));
	});
}


bool EU4::Regions::provinceInRegion(int province, const std::string& regionName) const
{
	auto region = regions.find(regionName);
	if (region != regions.end())
	{
		return region->second.containsProvince(province);
	}
	else
	{
		return false;
	}
}