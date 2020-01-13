#include "Region.h"
#include "AreaNames.h"
#include "Areas.h"
#include "ParserHelpers.h"

EU4::Region::Region(std::istream& theStream)
{
	registerKeyword("areas", [this](const std::string& unused, std::istream& theStream) 
		{
			AreaNames names(theStream);
			auto newNames = names.getNames();
			areaNames.merge(newNames);
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}

EU4::Region::Region(const std::set<int>& _provinces):
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