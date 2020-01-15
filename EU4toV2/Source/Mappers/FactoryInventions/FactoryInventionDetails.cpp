#include "FactoryInventionDetails.h"
#include "ParserHelpers.h"

mappers::FactoryInventionDetails::FactoryInventionDetails(std::istream& theStream)
{
	registerRegex("activate_building", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString buildingStr(theStream);
			factoryName = buildingStr.getString();
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
