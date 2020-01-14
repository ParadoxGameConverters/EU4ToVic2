#include "FactoryStartingCounts.h"
#include "ParserHelpers.h"
#include "Log.h"

mappers::FactoryStartingCounts::FactoryStartingCounts()
{
	LOG(LogLevel::Info) << "\tParsing starting factories.";

	registerRegex("[a-z_]+", [this](const std::string& factoryName, std::istream& theStream)
		{
			commonItems::singleInt countInt(theStream);
			startingCounts.insert(std::make_pair(factoryName, countInt.getInt()));
		});
	registerRegex("[a-zA-Z0-9\\_]+", commonItems::ignoreItem);

	parseFile("configurables/starting_factories.txt");
	clearRegisteredKeywords();
}
