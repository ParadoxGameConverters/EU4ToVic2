#include "EU4Empire.h"
#include "ParserHelpers.h"

EU4::EU4Empire::EU4Empire(std::istream& theStream)
{
	registerKeywords();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU4::EU4Empire::registerKeywords()
{
	registerKeyword("emperor", [this](const std::string& unused, std::istream& theStream) {
		emperor = commonItems::singleString(theStream).getString();
	});
	registerKeyword("passed_reform", [this](const std::string& unused, std::istream& theStream) {
		reforms.insert(commonItems::singleString(theStream).getString());
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
