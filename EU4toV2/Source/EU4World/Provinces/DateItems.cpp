#include "DateItems.h"
#include "ParserHelpers.h"

EU4::DateItems::DateItems(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU4::DateItems::registerKeys()
{
	registerRegex("owner|culture|religion", [this](const std::string& changeType, std::istream& theStream) {
		dateChanges.emplace_back(DateChange(changeType, commonItems::singleString(theStream).getString()));
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
