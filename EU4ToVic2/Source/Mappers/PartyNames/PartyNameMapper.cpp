#include "PartyNameMapper.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

mappers::PartyNameMapper::PartyNameMapper()
{
	registerKeys();
	parseFile("configurables/party_names.txt");
	clearRegisteredKeywords();
}

mappers::PartyNameMapper::PartyNameMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::PartyNameMapper::registerKeys()
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& party, std::istream& theStream) {
		const PartyName langToName(theStream);
		partyToLanguageMap.insert(make_pair(party, langToName));
	});
}
