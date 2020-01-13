#include "EU4Regiment.h"
#include "ParserHelpers.h"

EU4::EU4Regiment::EU4Regiment(std::istream& theStream)
{
	registerKeyword("id", [this](const std::string& unused, std::istream& theStream)
		{
			EU4::ID idblock(theStream);
			regimentId = idblock;
		});
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString nameStr(theStream);
			name = nameStr.getString();
		});
	registerKeyword("type", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString typeStr(theStream);
			regimentType = typeStr.getString();
		});
	registerKeyword("home", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt homeInt(theStream);
			home = homeInt.getInt();
		});
	registerKeyword("morale", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleDouble moraleDuble(theStream);
			morale = moraleDuble.getDouble();
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
