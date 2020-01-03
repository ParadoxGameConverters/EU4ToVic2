#include "V2FactoryTypeLoader.h"
#include "V2FactoryInventionLoader.h"
#include "V2FactoryTechLoader.h"
#include "V2FactoryType.h"
#include "V2FactoryStartingCounts.h"
#include "ParserHelpers.h"
#include "../../Configuration.h"


mappers::V2FactoryTypeLoader::V2FactoryTypeLoader()
{
	registerKeyword(std::regex("[a-z_]+"), [this](const std::string& factoryName, std::istream& theStream)
		{
			V2FactoryType factoryType(theStream);
			factoryType.setFactoryTypeName(factoryName);
			factoryTypes.insert(std::make_pair(factoryName, factoryType));
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.]+"), commonItems::ignoreItem);

	parseFile(theConfiguration.getVic2Path() + "/common/production_types.txt");

	V2FactoryStartingCounts factoryStartingCounts;
	V2FactoryTechLoader factoryTechs;
	V2FactoryInventionLoader factoryInventions;

	std::map<std::string, V2FactoryType>::iterator factoryTypeItr;
	for (factoryTypeItr = factoryTypes.begin(); factoryTypeItr != factoryTypes.end(); ++factoryTypeItr)
	{
		factoryTypeItr->second.assignTechsAndCounts(
			factoryTechs.getFactoryTechMap(), 
			factoryInventions.getFactoryInventionMap(), 
			factoryStartingCounts.getFactoryStartingCounts()
		);
	}
}
