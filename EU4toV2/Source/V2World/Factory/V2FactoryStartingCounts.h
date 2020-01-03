#ifndef V2_FACTORY_STARTING_COUNTS_H_
#define V2_FACTORY_STARTING_COUNTS_H_

#include "newParser.h"
#include <map>


namespace mappers
{

	class V2FactoryStartingCounts : commonItems::parser
	{
	public:
		V2FactoryStartingCounts();
		std::map<std::string, int> getFactoryStartingCounts() const { return startingCounts; }

	private:
		std::map<std::string, int> startingCounts;
	};
}

#endif // V2_FACTORY_STARTING_COUNTS_H_