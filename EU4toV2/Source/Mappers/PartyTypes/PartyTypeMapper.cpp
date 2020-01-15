#include "PartyTypeMapper.h"
#include <Log.h>
#include "ParserHelpers.h"

mappers::PartyTypeMapper::PartyTypeMapper()
{
	Log(LogLevel::Info) << "Parsing party detail blobs.";
	registerKeys();
	parseFile("configurables/party_blobs.txt");
	clearRegisteredKeywords();
}

mappers::PartyTypeMapper::PartyTypeMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::PartyTypeMapper::registerKeys()
{
	registerRegex("[a-z_]+", [this](const std::string& partyIdeology, std::istream& theStream) 
		{
			PartyType newType(theStream);
			newType.setIdeology(partyIdeology);
			partyTypeMap.insert(std::make_pair(partyIdeology, newType));
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}

std::optional<mappers::PartyType> mappers::PartyTypeMapper::getPartyTypeByIdeology(const std::string& ideology) const
{
	const auto& partyTypeItr = partyTypeMap.find(ideology);
	if (partyTypeItr != partyTypeMap.end()) return partyTypeItr->second;
	return std::nullopt;
}
