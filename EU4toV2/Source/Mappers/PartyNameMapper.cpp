#include "PartyNameMapper.h"
#include <Log.h>

mappers::PartyNameMapper::PartyNameMapper(std::istream& theStream)
{
	LOG(LogLevel::Debug) << "Getting Party Names";

	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this](const std::string& party, std::istream& theStream) {
		PartyName langToName(theStream);

		partyToLanguageMap.insert(make_pair(party, langToName));
	});

	parseStream(theStream);
}
