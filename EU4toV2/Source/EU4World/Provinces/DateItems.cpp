#include "DateItems.h"
#include "ParserHelpers.h"

EU4::DateItems::DateItems(std::istream& theStream)
{
	registerRegex("owner|culture|religion", [this](const std::string& changeType, std::istream& theStream)
		{
			commonItems::singleString changeStr(theStream);
			dateChanges.push_back(std::make_pair(EU4::DateItemNames[changeType], changeStr.getString()));
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}