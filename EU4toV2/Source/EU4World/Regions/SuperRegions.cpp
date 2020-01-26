#include "SuperRegions.h"
#include "ParserHelpers.h"

EU4::SuperRegions::SuperRegions(std::istream& theStream)
{
	registerRegex("\\w+_superregion", [this](const std::string& sregionName, std::istream& theStream)
		{
			const commonItems::stringList regionList(theStream);
			superRegionMap[sregionName] = regionList.getStrings();
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
