#include "ProvinceMapping.h"
#include "ParserHelpers.h"

mappers::ProvinceMapping::ProvinceMapping(std::istream& theStream)
{
	registerKeyword("eu4", [this](const std::string & unused, std::istream & theStream) 
		{
			const commonItems::singleInt provinceInt(theStream);
			eu4Provinces.push_back(provinceInt.getInt());
		});
	registerKeyword("v2", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleInt provinceInt(theStream);
			vic2Provinces.push_back(provinceInt.getInt());
		});
	registerKeyword("resettable", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString regionString(theStream);
			resettableRegions.insert(regionString.getString());
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}