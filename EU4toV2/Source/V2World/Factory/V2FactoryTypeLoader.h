#ifndef V2_FACTORY_TYPE_LOADER_H_
#define V2_FACTORY_TYPE_LOADER_H_

#include "newParser.h"
#include "V2FactoryType.h"
#include <map>

namespace mappers
{
	class V2FactoryTypeLoader : commonItems::parser
	{
	public:
		V2FactoryTypeLoader();
		std::map<std::string, V2FactoryType> getFactoryTypes() const { return factoryTypes; }

	private:
		std::map<std::string, V2FactoryType> factoryTypes;
	};
}

#endif // V2_FACTORY_TYPE_LOADER_H_