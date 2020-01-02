#include "V2FactoryTechDetails.h"
#include "ParserHelpers.h"
#include "Log.h"


mappers::V2FactoryTechDetails::V2FactoryTechDetails(std::istream& theStream)
{
	registerKeyword(std::regex("activate_building"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString buildingStr(theStream);
			factoryName = buildingStr.getString();
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_]+"), commonItems::ignoreItem);

	parseStream(theStream);
}
