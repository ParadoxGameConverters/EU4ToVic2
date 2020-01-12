#include "ProvinceMapping.h"
#include "ParserHelpers.h"

mappers::ProvinceMapping::ProvinceMapping(std::istream& theStream)
{
	registerKeyword("eu4", [this](const std::string & unused, std::istream & theStream) 
		{
			commonItems::singleInt provinceInt(theStream);
			EU4Provinces.push_back(provinceInt.getInt());
		});
	registerKeyword("v2", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt provinceInt(theStream);
			Vic2Provinces.push_back(provinceInt.getInt());
		});
	registerKeyword("resettable", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString regionString(theStream);
			resettableRegions.insert(regionString.getString());
		});

	parseStream(theStream);

	if (EU4Provinces.empty())
	{
		EU4Provinces.push_back(0);
	}
	if (Vic2Provinces.empty())
	{
		Vic2Provinces.push_back(0);
	}
}