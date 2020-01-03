#ifndef V2FACTORY_FACTORY_H_
#define V2FACTORY_FACTORY_H_

#include <deque>
#include "V2Factory.h"
#include "V2FactoryType.h"


class V2FactoryFactory
{
	public:
		V2FactoryFactory();
		std::deque<V2Factory> buildFactories() const;

	private:
		std::map<std::string, mappers::V2FactoryType> factoryTypes;
};

#endif // V2FACTORY_FACTORY_H_