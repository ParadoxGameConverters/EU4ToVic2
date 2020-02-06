#include "Regions.h"
#include "Areas.h"
#include "Log.h"

EU4::Regions::Regions(const SuperRegions& sRegions, const Areas& areas, std::istream& regionsFile)
{
	registerRegex("\\w+_region", [this, areas](const std::string& regionName, std::istream& areasFile)
		{
			Region newRegion(areasFile);
			newRegion.addProvinces(areas);
			regions.insert(make_pair(regionName, newRegion));
		});

	parseStream(regionsFile);
	clearRegisteredKeywords();

	for (const auto& area : areas.getAreas()) regions.insert(std::make_pair(area.first, Region(area.second)));
	superRegions = sRegions.getSuperRegions();
}

EU4::Regions::Regions(const Areas& areas)
{
	auto theAreas = areas.getAreas();
	std::for_each(theAreas.begin(), theAreas.end(), [this](const std::pair<std::string, std::set<int>> & theArea)
	{
		regions.insert(make_pair(theArea.first, Region(theArea.second)));
	});
}

bool EU4::Regions::provinceInRegion(int province, const std::string& regionName) const
{
	const auto& regionItr = regions.find(regionName);
	if (regionItr != regions.end()) return regionItr->second.regionContainsProvince(province);

	// "Regions" are such a fluid term.
	const auto& superRegionItr = superRegions.find(regionName);
	if (superRegionItr != superRegions.end())
	{
		for (const auto& regionalName: superRegionItr->second)
		{
			const auto& regionalItr = regions.find(regionalName);
			if (regionalItr->second.regionContainsProvince(province)) return true;
		}
	}

	// And sometimes they don't mean what people think they mean at all.
	for (const auto& region: regions)
	{
		for (const auto& areaName: region.second.getAreaNames())
		{
			if (areaName == regionName)
			{
				return region.second.areaContainsProvince(areaName, province);
			}
		}
	}

	return false;
}

std::optional<std::string> EU4::Regions::getParentAreaName(const int provinceID) const
{	
	for (const auto& region: regions)
	{
		for (const auto& areaName: region.second.getAreaNames())
		{
			if (region.second.areaContainsProvince(areaName, provinceID)) return areaName;
		}
	}
	Log(LogLevel::Warning) << "Province ID " + std::to_string(provinceID) + " has no parent area name! (Area mismatch? Using newer EU4 version to convert older save?)";
	return std::nullopt;
}

std::optional<std::string> EU4::Regions::getParentRegionName(const int provinceID) const
{	
	for (const auto& region: regions)
	{
		if (region.second.regionContainsProvince(provinceID)) return region.first;
	}
	Log(LogLevel::Warning) << "Province ID " + std::to_string(provinceID) + " has no parent region name! (Area mismatch? Using newer EU4 version to convert older save?)";
	return std::nullopt;
}

std::optional<std::string> EU4::Regions::getParentSuperRegionName(const int provinceID) const
{	
	for (const auto& superRegion: superRegions)
	{
		for (const auto& regionalName: superRegion.second)
		{
			const auto& regionalItr = regions.find(regionalName);
			if (regionalItr->second.regionContainsProvince(provinceID)) return superRegion.first;
		}
	}
	Log(LogLevel::Warning) << "Province ID " + std::to_string(provinceID) + " has no parent superregion name! (Area mismatch? Using newer EU4 version to convert older save?)";
	return std::nullopt;
}

bool EU4::Regions::regionIsValid(const std::string& regionName) const
{
	const auto& regionItr = regions.find(regionName);
	if (regionItr != regions.end()) return true;
	
	// Who knows what the mapper needs. All kinds of stuff.
	const auto& superRegionItr = superRegions.find(regionName);
	if (superRegionItr != superRegions.end()) return true;
	
	// And more stuff, what's the worst that could happen?
	for (const auto& region : regions) if (region.second.getAreaNames().count(regionName)) return true;

	return false;
}