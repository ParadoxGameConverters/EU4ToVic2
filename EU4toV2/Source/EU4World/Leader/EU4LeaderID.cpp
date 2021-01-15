#include "EU4LeaderID.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "CommonRegexes.h"

EU4::LeaderID::LeaderID(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU4::LeaderID::registerKeys()
{
	registerKeyword("id", [this](const std::string& unused, std::istream& theStream) {
		IDNum = commonItems::singleInt(theStream).getInt();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
