#ifndef CONTINENTS_H
#define CONTINENTS_H

#include "Parser.h"
#include <map>
#include <optional>

namespace mappers
{
	class Continents: commonItems::parser
	{
	public:
		Continents();
		explicit Continents(std::istream& theStream);
		
		[[nodiscard]] std::optional<std::string> getEU4Continent(int eu4Province) const;

	private:
		void registerKeys();
		std::map<int, std::string> continentMap;
	};
}

#endif // CONTINENTS_H
