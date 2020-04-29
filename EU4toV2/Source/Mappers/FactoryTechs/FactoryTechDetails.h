#ifndef FACTORY_TECH_DETAILS_H
#define FACTORY_TECH_DETAILS_H

#include "Parser.h"

namespace mappers
{
	class FactoryTechDetails : commonItems::parser
	{
	public:
		explicit FactoryTechDetails(std::istream& theStream);
		
		[[nodiscard]] const auto& getFactoryName() const { return factoryName; }

	private:
		std::string factoryName;
	};
}

#endif // FACTORY_TECH_DETAILS_H