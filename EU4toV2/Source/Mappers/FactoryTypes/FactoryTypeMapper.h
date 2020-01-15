#ifndef FACTORY_TYPE_LOADER_H
#define FACTORY_TYPE_LOADER_H

#include "newParser.h"
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
		std::map<std::string, FactoryType> getFactoryTypes() const { return factoryTypes; }
		std::deque<V2::Factory> buildFactories() const;

	private:
		std::map<std::string, FactoryType> factoryTypes;

		FactoryStartingCounts factoryStartingCounts;
		FactoryTechMapper factoryTechs;
		FactoryInventionMapper factoryInventions;
	};
}



#endif // FACTORY_TYPE_LOADER_H