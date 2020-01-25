#include "EU4Regiment.h"
#include "ParserHelpers.h"

EU4::EU4Regiment::EU4Regiment(std::istream& theStream)
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString nameStr(theStream);
			name = nameStr.getString();
		});
	registerKeyword("type", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString typeStr(theStream);
			regimentType = typeStr.getString();
		});
	registerKeyword("home", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleInt homeInt(theStream);
			home = homeInt.getInt();
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
