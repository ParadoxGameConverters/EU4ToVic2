#ifndef V2_FACTORY_TECH_DETAILS_H_
#define V2_FACTORY_TECH_DETAILS_H_

#include "newParser.h"


namespace mappers
{
	class V2FactoryTechDetails : commonItems::parser
	{
	public:
		V2FactoryTechDetails(std::istream& theStream);
		std::string getFactoryName() const { return factoryName; }

	private:
		std::string factoryName;
	};
}

#endif // V2_FACTORY_TECH_DETAILS_H_