#include "FactoryTypeMapper.h"
#include "ParserHelpers.h"
#include "../../Configuration.h"
#include "Log.h"

mappers::FactoryTypeMapper::FactoryTypeMapper()
{
	LOG(LogLevel::Info) << "Determining factory allocation rules.";

	registerRegex("[a-z_]+", [this](const std::string& factoryName, std::istream& theStream)
		{
			FactoryType factoryType(theStream);
			factoryType.setFactoryTypeName(factoryName);
			factoryTypes.insert(std::make_pair(factoryName, factoryType));
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseFile(theConfiguration.getVic2Path() + "/common/production_types.txt");
	clearRegisteredKeywords();

	for (auto& factoryTypeItr : factoryTypes)
	{
		factoryTypeItr.second.assignTechsAndCounts(
			factoryTechs.getFactoryTechMap(), 
			factoryInventions.getFactoryInventionMap(), 
			factoryStartingCounts.getFactoryStartingCounts()
		);
	}
}

std::deque<V2Factory> mappers::FactoryTypeMapper::buildFactories() const
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
