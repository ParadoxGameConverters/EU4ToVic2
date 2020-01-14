#ifndef FACTORY_STARTING_COUNTS_H
#define FACTORY_STARTING_COUNTS_H

#include "newParser.h"
#include <map>

namespace mappers
{
	class FactoryStartingCounts : commonItems::parser
	{
	public:
		FactoryStartingCounts();
		std::map<std::string, int> getFactoryStartingCounts() const { return startingCounts; }

	private:
		std::map<std::string, int> startingCounts;
	};
}

#endif // FACTORY_STARTING_COUNTS_H