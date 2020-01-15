#ifndef FACTORY_INVENTION_DETAILS_H
#define FACTORY_INVENTION_DETAILS_H

#include "newParser.h"

namespace mappers
{
	class FactoryInventionDetails : commonItems::parser
	{
	public:
		FactoryInventionDetails(std::istream& theStream);
		std::string getFactoryName() const { return factoryName; }

	private:
		std::string factoryName;		
	};
}

#endif // FACTORY_INVENTION_DETAILS_H