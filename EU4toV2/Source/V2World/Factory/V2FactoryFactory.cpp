#include "V2FactoryFactory.h"
#include "V2FactoryTypeLoader.h"


V2FactoryFactory::V2FactoryFactory()
{
	mappers::V2FactoryTypeLoader factoryTypeLoader;
	factoryTypes = factoryTypeLoader.getFactoryTypes();
}

std::deque<V2Factory> V2FactoryFactory::buildFactories() const
{
	std::deque<V2Factory> factoryQueue;
	for (auto factoryTypeItr : factoryTypes)
	{
		for (int counter = 0; counter < factoryTypeItr.second.getStartingCount(); ++counter)
		{
			V2Factory newFactory(factoryTypeItr.second);
			factoryQueue.push_back(newFactory);
		}
	}

	return factoryQueue;
}
