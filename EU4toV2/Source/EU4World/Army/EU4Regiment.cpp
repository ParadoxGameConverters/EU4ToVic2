#include "EU4Regiment.h"
#include "ParserHelpers.h"


EU4::EU4Regiment::EU4Regiment(std::istream& theStream)
{
	registerKeyword(std::regex("id"), [this](const std::string& unused, std::istream& theStream)
		{
			EU4::EU4UnitID idblock(theStream);
			regimentId = idblock;
		});
	registerKeyword(std::regex("name"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString nameStr(theStream);
			name = nameStr.getString();
		});
	registerKeyword(std::regex("type"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString typeStr(theStream);
			regimentType = typeStr.getString();
		});
	registerKeyword(std::regex("home"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt homeInt(theStream);
			home = homeInt.getInt();
		});
	registerKeyword(std::regex("morale"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleDouble moraleDuble(theStream);
			morale = moraleDuble.getDouble();
		});
	registerKeyword(std::regex("[a-z0-9\\_]+"), commonItems::ignoreItem);

	parseStream(theStream);
}
