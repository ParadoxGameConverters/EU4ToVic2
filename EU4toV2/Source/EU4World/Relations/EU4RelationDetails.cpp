#include "EU4RelationDetails.h"
#include "ParserHelpers.h"

EU4::EU4RelationDetails::EU4RelationDetails(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU4::EU4RelationDetails::registerKeys()
{
	registerRegex("value|cached_sum", [this](const std::string& unused, std::istream& theStream) {
		value = commonItems::singleInt(theStream).getInt();
	});
	registerKeyword("military_access", [this](const std::string& unused, std::istream& theStream) {
		commonItems::ignoreItem(unused, theStream);
		military_access = true;
	});
	registerKeyword("attitude", [this](const std::string& unused, std::istream& theStream) {
		attitude = commonItems::singleString(theStream).getString();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
