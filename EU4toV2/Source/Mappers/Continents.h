#ifndef CONTINENTS_H
#define CONTINENTS_H

#include "newParser.h"
#include <map>
#include <optional>

namespace mappers
{
	class Continents: commonItems::parser
	{
	public:
		Continents();
		std::optional<std::string> getEU4Continent(int EU4Province);

	private:
		std::map<int, std::string> continentMap;
	};
}

#endif // CONTINENTS_H
