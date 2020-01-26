#include "PartyName.h"
#include "ParserHelpers.h"

mappers::PartyName::PartyName(std::istream& theStream)
{
	registerRegex("[a-zA-Z0-9_]+", [this](const std::string& language, std::istream& theStream) {
		const commonItems::singleString incomingStr(theStream);
		languageToNameMap.insert(std::make_pair(language, incomingStr.getString()));
	});

	parseStream(theStream);
	clearRegisteredKeywords();
}
