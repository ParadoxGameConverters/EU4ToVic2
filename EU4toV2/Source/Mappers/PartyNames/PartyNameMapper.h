#ifndef PARTY_NAME_MAPPER_H
#define PARTY_NAME_MAPPER_H

#include "newParser.h"
#include "PartyName.h"
#include <map>
#include <string>

namespace mappers
{
	class PartyNameMapper: commonItems::parser
	{
	public:
		PartyNameMapper();
		PartyNameMapper(std::istream& theStream);
		std::map<std::string, mappers::PartyName> getMap() const { return partyToLanguageMap; }

	private:
		void registerKeys();
		std::map<std::string, mappers::PartyName> partyToLanguageMap;
	};
}

#endif // PARTY_NAME_MAPPER_H