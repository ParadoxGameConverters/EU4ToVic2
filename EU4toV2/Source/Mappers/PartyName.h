#ifndef PARTY_NAME_H
#define PARTY_NAME_H

#include "newParser.h"
#include "ParserHelpers.h"
#include <map>
#include <optional>

namespace mappers
{
	class PartyName : commonItems::parser
	{
	public:
		PartyName(std::istream& theStream);
		std::optional<std::string> mappers::PartyName::getLanguageToName(const std::string& language) const;
		std::map<std::string, std::string> getMap() const { return languageToNameMap; }

	private:
		std::map<std::string, std::string> languageToNameMap;
	};

}

#endif // PARTY_NAME_H