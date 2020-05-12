#ifndef FACTORY_INVENTION_DETAILS_H
#define FACTORY_INVENTION_DETAILS_H

#include "Parser.h"

namespace mappers
{
	class FactoryInventionDetails : commonItems::parser
	{
	public:
		explicit FactoryInventionDetails(std::istream& theStream);
		
		[[nodiscard]] const auto& getFactoryName() const { return factoryName; }

	private:
		std::string factoryName;		
	};
}

#endif // FACTORY_INVENTION_DETAILS_H