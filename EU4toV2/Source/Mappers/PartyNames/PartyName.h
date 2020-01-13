#ifndef PARTY_NAME_H
#define PARTY_NAME_H

#include "newParser.h"
#include <map>

namespace mappers
{
	class PartyName : commonItems::parser
	{
	public:
		PartyName(std::istream& theStream);
		std::map<std::string, std::string> getMap() const { return languageToNameMap; }

	private:
		std::map<std::string, std::string> languageToNameMap;
	};
}

#endif // PARTY_NAME_H