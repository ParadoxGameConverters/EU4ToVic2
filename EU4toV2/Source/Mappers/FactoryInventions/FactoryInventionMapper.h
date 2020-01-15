#ifndef FACTORY_INVENTION_MAPPER_H
#define FACTORY_INVENTION_MAPPER_H

#include "newParser.h"
#include <map>

namespace mappers
{

	class FactoryInventionMapper : commonItems::parser
	{
	public:
		FactoryInventionMapper();
		FactoryInventionMapper(std::istream& theStream);
		const std::map<std::string, std::string>& getFactoryInventionMap() const { return factoryInventionMap; }

	private:
		void registerKeys();
		std::map<std::string, std::string> factoryInventionMap;
	};
}

#endif // FACTORY_INVENTION_MAPPER_H