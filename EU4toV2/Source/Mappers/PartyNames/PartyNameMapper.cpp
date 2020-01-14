#include "PartyNameMapper.h"
#include <Log.h>

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
	registerRegex("[a-zA-Z0-9_]+", [this](const std::string& party, std::istream& theStream) 
		{
			PartyName langToName(theStream);
			partyToLanguageMap.insert(make_pair(party, langToName));
		});
}