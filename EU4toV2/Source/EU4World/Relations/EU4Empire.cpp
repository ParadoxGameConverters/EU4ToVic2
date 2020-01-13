#include "EU4Empire.h"
#include "ParserHelpers.h"

EU4::EU4Empire::EU4Empire(std::istream& theStream)
{
	registerKeyword("emperor", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString emperorStr(theStream);
			emperor = emperorStr.getString();
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
