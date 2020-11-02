#include "EU4Modifier.h"
#include "ParserHelpers.h"

EU4::EU4Modifier::EU4Modifier(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU4::EU4Modifier::registerKeys()
{
	registerKeyword("modifier", [this](const std::string& unused, std::istream& theStream) {
		modifier = commonItems::singleString(theStream).getString();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
