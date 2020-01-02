#ifndef V2_FACTORY_INVENTION_DETAILS_H_
#define V2_FACTORY_INVENTION_DETAILS_H_

#include "newParser.h"


namespace mappers
{
	class V2FactoryInventionDetails : commonItems::parser
	{
	public:
		V2FactoryInventionDetails(std::istream& theStream);
		std::string getFactoryName() const { return factoryName; }

	private:
		std::string factoryName;		
	};
}

#endif // V2_FACTORY_INVENTION_DETAILS_H_