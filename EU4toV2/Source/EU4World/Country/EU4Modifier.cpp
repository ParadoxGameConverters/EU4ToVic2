#include "EU4Modifier.h"
#include "ParserHelpers.h"

EU4::EU4Modifier::EU4Modifier(std::istream& theStream)
{
	registerKeyword(std::regex("modifier"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString modifierStr(theStream);
			modifier = modifierStr.getString();
		});
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}
