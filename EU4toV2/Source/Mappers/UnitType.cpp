#include "UnitType.h"
#include "ParserHelpers.h"
#include "Log.h"


mappers::UnitType::UnitType(std::istream& theStream)
{
	registerKeyword(std::regex("maneuver|offensive_morale|defensive_morale|offensive_fire|defensive_fire|offensive_shock|defensive_shock|hull_size"), 
		[this](const std::string& unused, std::istream& theStream)
	{
		commonItems::singleInt strInt(theStream);
		strength += strInt.getInt();
	});
	registerKeyword(std::regex("type"), [this](const std::string& unused, std::istream& theStream)
	{
		commonItems::singleString typeStr(theStream);
		try
		{
			unitType = EU4::RegimentCategoryNames[typeStr.getString()];
		}
		catch (std::exception e)
		{
			std::runtime_error exception("Illegal unit type: " + typeStr.getString() + ", aborting!");
			throw exception;
		}
	});
	registerKeyword(std::regex("[a-z0-9\\_]+"), commonItems::ignoreItem);

	parseStream(theStream);
}