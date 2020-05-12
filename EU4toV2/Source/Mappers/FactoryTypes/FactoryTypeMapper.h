#ifndef FACTORY_TYPE_LOADER_H
#define FACTORY_TYPE_LOADER_H

#include "Parser.h"
#include "FactoryType.h"
#include "../FactoryInventions/FactoryInventionMapper.h"
#include "../FactoryTechs/FactoryTechMapper.h"
#include "../FactoryCounts/FactoryStartingCounts.h"
#include <map>
#include <deque>
#include "../../V2World/Factory/Factory.h"

namespace mappers
{
	class FactoryTypeMapper : commonItems::parser
	{
	public:
		FactoryTypeMapper();
		
		[[nodiscard]] const auto& getFactoryTypes() const { return factoryTypes; }
		[[nodiscard]] std::deque<std::shared_ptr<V2::Factory>> buildFactories() const;

	private:
		std::map<std::string, FactoryType> factoryTypes;

		FactoryStartingCounts factoryStartingCounts;
		FactoryTechMapper factoryTechs;
		FactoryInventionMapper factoryInventions;
	};
}



#endif // FACTORY_TYPE_LOADER_H