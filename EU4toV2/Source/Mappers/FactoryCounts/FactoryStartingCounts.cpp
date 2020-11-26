#include "FactoryStartingCounts.h"
#include "Log.h"
#include "ParserHelpers.h"

mappers::FactoryStartingCounts::FactoryStartingCounts()
{
	LOG(LogLevel::Info) << "Parsing starting factories.";
	registerKeywords();
	parseFile("configurables/starting_factories.txt");
	clearRegisteredKeywords();
}

mappers::FactoryStartingCounts::FactoryStartingCounts(std::istream& theStream)
{
	registerKeywords();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::FactoryStartingCounts::registerKeywords()
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& factoryName, std::istream& theStream) {
		startingCounts.insert(std::make_pair(factoryName, commonItems::singleInt(theStream).getInt()));
	});
}
