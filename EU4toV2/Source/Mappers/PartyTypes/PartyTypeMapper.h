#ifndef PARTY_TYPE_MAPPER_H
#define PARTY_TYPE_MAPPER_H

#include "newParser.h"
#include "PartyType.h"
#include <map>
#include <string>
#include <optional>

namespace mappers
{
	class PartyTypeMapper: commonItems::parser
	{
	public:
		PartyTypeMapper();
		PartyTypeMapper(std::istream& theStream);
		std::optional<mappers::PartyType> getPartyTypeByIdeology(const std::string& ideology) const;

	private:
		void registerKeys();
		std::map<std::string, PartyType> partyTypeMap;
	};
}

#endif // PARTY_TYPE_MAPPER_H