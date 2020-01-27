#ifndef FACTORY_TECH_MAPPER_H
#define FACTORY_TECH_MAPPER_H

#include "newParser.h"
#include <map>

namespace mappers
{
	class FactoryTechMapper : commonItems::parser
	{
	public:
		FactoryTechMapper();
		explicit FactoryTechMapper(std::istream& theStream);
		
		[[nodiscard]] const auto& getFactoryTechMap() const { return factoryTechMap; }

	private:
		void registerKeys();
		
		std::map<std::string, std::string> factoryTechMap;
	};
}

#endif // FACTORY_TECH_MAPPER_H