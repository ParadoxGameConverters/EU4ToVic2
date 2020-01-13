#include "V2FactoryStartingCounts.h"
#include "ParserHelpers.h"


mappers::V2FactoryStartingCounts::V2FactoryStartingCounts()
{
	registerKeyword(std::regex("[a-z_]+"), [this](const std::string& factoryName, std::istream& theStream)
		{
			commonItems::singleInt countInt(theStream);
			startingCounts.insert(std::make_pair(factoryName, countInt.getInt()));
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_]+"), commonItems::ignoreItem);

	parseFile("configurables/starting_factories.txt");
}
