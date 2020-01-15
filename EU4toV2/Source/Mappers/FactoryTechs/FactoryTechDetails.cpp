#include "FactoryTechDetails.h"
#include "ParserHelpers.h"
#include "Log.h"

mappers::FactoryTechDetails::FactoryTechDetails(std::istream& theStream)
{
	registerKeyword("activate_building", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString buildingStr(theStream);
			factoryName = buildingStr.getString();
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
