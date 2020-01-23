#include "Region.h"
#include "AreaNames.h"
#include "Areas.h"
#include "ParserHelpers.h"

EU4::Region::Region(std::istream& theStream)
{
	registerKeyword("areas", [this](const std::string& unused, std::istream& theStream) 
		{
			const AreaNames names(theStream);
			areaNames = names.getNames();
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}

EU4::Region::Region(const std::set<int>& _provinces)
{
	areaProvinces["dummyArea"] = _provinces;
	areaNames.insert("dummyArea");
}

bool EU4::Region::regionContainsProvince(int province) const
{
	for (const auto& area: areaProvinces)
	{
		if (area.second.count(province)) return true;
	}
	return false;
}

bool EU4::Region::areaContainsProvince(const std::string& areaName, int province) const
{
	// Support for old installations without areas:
	const auto& areaItr = areaProvinces.find("dummyArea");
	if (areaItr != areaProvinces.end()) if (areaItr->second.count(province)) return true;
	
	const auto& areaItr2 = areaProvinces.find(areaName);
	if (areaItr2 == areaProvinces.end()) return false;
	if (areaItr2->second.count(province)) return true;
	return false;
}

void EU4::Region::addProvinces(const Areas& areas)
{
	for (const auto& areaName: areaNames)
	{
		const auto& newProvinces = areas.getProvincesInArea(areaName);
		areaProvinces[areaName] = newProvinces;
	}
}