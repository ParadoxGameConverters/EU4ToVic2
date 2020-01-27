#include "DateItems.h"
#include "ParserHelpers.h"

EU4::DateItems::DateItems(std::istream& theStream)
{
	registerRegex("owner|culture|religion", [this](const std::string& changeType, std::istream& theStream)
		{
			const commonItems::singleString changeStr(theStream);
			dateChanges.emplace_back(std::make_pair(DateItemNames[changeType], changeStr.getString()));
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}