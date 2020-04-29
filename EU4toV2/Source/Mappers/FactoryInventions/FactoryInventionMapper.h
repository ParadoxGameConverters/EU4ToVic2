#ifndef FACTORY_INVENTION_MAPPER_H
#define FACTORY_INVENTION_MAPPER_H

#include "Parser.h"
#include <map>

namespace mappers
{

	class FactoryInventionMapper : commonItems::parser
	{
	public:
		FactoryInventionMapper();
		explicit FactoryInventionMapper(std::istream& theStream);
		
		[[nodiscard]] const auto& getFactoryInventionMap() const { return factoryInventionMap; }

	private:
		void registerKeys();
		
		std::map<std::string, std::string> factoryInventionMap;
	};
}

#endif // FACTORY_INVENTION_MAPPER_H