#include "V2FactoryInventionDetails.h"
#include "ParserHelpers.h"


mappers::V2FactoryInventionDetails::V2FactoryInventionDetails(std::istream& theStream)
{
	registerKeyword(std::regex("activate_building"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString buildingStr(theStream);
			factoryName = buildingStr.getString();
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}
