#include "EU4UnitID.h"
#include "ParserHelpers.h"


EU4::EU4UnitID::EU4UnitID(std::istream& theStream)
{
	registerKeyword(std::regex("id"), [this](const std::string& unused, std::istream& theStream)
	{
		commonItems::singleInt idInt(theStream);
		unitId = idInt.getInt();
	});
	registerKeyword(std::regex("type"), [this](const std::string& unused, std::istream& theStream)
	{
		commonItems::singleInt typeInt(theStream);
		unitType = typeInt.getInt();
	});
	registerKeyword(std::regex("[a-z0-9\\_]+"), commonItems::ignoreItem);

	parseStream(theStream);
}

