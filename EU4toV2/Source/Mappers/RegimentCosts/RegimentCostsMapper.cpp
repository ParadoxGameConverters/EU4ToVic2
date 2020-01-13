#include "RegimentCostsMapper.h"
#include "ParserHelpers.h"
#include "Log.h"

mappers::RegimentCostsMapper::RegimentCostsMapper()
{
	LOG(LogLevel::Info) << "Parsing regiment costs";
	registerKeys();
	parseFile("configurables/regiment_costs.txt");
	clearRegisteredKeywords();
}

mappers::RegimentCostsMapper::RegimentCostsMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::RegimentCostsMapper::registerKeys()
{
	registerRegex("[a-z_]+", [this](const std::string& regiment, std::istream& theStream)
		{
			commonItems::singleInt costStr(theStream);
			regimentCosts.insert(std::make_pair(regiment, costStr.getInt()));
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}