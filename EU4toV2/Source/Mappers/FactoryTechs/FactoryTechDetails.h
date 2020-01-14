#ifndef FACTORY_TECH_DETAILS_H
#define FACTORY_TECH_DETAILS_H

#include "newParser.h"

namespace mappers
{
	class FactoryTechDetails : commonItems::parser
	{
	public:
		FactoryTechDetails(std::istream& theStream);
		std::string getFactoryName() const { return factoryName; }

	private:
		std::string factoryName;
	};
}

#endif // FACTORY_TECH_DETAILS_H