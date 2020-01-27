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

std::deque<std::shared_ptr<V2::Factory>> mappers::FactoryTypeMapper::buildFactories() const
{
	std::deque<std::shared_ptr<V2::Factory>> factoryQueue;
	for (auto factoryTypeItr : factoryTypes)
	{
		for (auto counter = 0; counter < factoryTypeItr.second.getStartingCount(); ++counter)
		{
			auto newFactory = std::make_shared<V2::Factory>(factoryTypeItr.second);
			factoryQueue.push_back(newFactory);
		}
	}

	return factoryQueue;
}
