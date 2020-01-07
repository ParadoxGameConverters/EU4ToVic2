#ifndef REGIMENT_COSTS_MAPPER_H
#define REGIMENT_COSTS_MAPPER_H

#include "newParser.h"
#include <map>

namespace mappers
{
	class RegimentCostsMapper : commonItems::parser
	{
	public:
		RegimentCostsMapper();
		std::map<std::string, int> getRegimentCosts() const { return regimentCosts; }

	private:
		std::map<std::string, int> regimentCosts;
	};
}

#endif // REGIMENT_COSTS_MAPPER_H
