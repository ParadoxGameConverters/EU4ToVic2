#include "Region.h"
#include "AreaNames.h"
#include "Areas.h"
#include "ParserHelpers.h"

EU4::Region::Region(std::istream& theStream)
{
	registerKeyword(std::regex("areas"), [this](const std::string& unused, std::istream& theStream) {
		AreaNames names(theStream);
		auto newNames = names.getNames();
		areaNames.merge(newNames);
	});
	registerKeyword(std::regex("discover_if"), commonItems::ignoreObject);
	registerKeyword(std::regex("monsoon"), commonItems::ignoreObject);

	parseStream(theStream);
}


EU4::Region::Region(std::set<int>& _provinces):
	provinces(_provinces)
{}


bool EU4::Region::containsProvince(unsigned int province) const
{
	return (provinces.count(province) > 0);
}


void EU4::Region::addProvinces(const EU4::Areas& areas)
{
	for (auto areaName: areaNames)
	{
		auto newProvinces = areas.getProvincesInArea(areaName);
		provinces.insert(newProvinces.begin(), newProvinces.end());
	}
}