#include "RegionProvinceMapper.h"
#include "../../Configuration.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

mappers::RegionProvinceMapper::RegionProvinceMapper()
{
	registerKeys();
	if (commonItems::DoesFileExist("./blankMod/output/map/region.txt"))
	{
		parseFile("./blankMod/output/map/region.txt");
	}
	else
	{
		parseFile(theConfiguration.getVic2Path() + "/map/region.txt");
	}
	clearRegisteredKeywords();
}

mappers::RegionProvinceMapper::RegionProvinceMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::RegionProvinceMapper::registerKeys()
{
	registerRegex("\\w\\w\\w_\\d+", [this](const std::string& regionName, std::istream& theStream)
		{
			const commonItems::intList provinceList(theStream);
			auto provinceVector = provinceList.getInts();
			std::set<int> provinces(provinceVector.begin(), provinceVector.end());
			theRegions.insert(make_pair(regionName, provinces));
		});
}

bool mappers::RegionProvinceMapper::provinceIsInRegion(int province, const std::string& region) const
{
	const auto provinces = theRegions.find(region);
	if (provinces != theRegions.end()) return provinces->second.count(province) > 0;
	return false;
}

std::set<int> mappers::RegionProvinceMapper::getProvincesInRegion(const std::string& region) const
{
	const auto provinces = theRegions.find(region);
	if (provinces != theRegions.end()) return provinces->second;
	return std::set<int>();
}