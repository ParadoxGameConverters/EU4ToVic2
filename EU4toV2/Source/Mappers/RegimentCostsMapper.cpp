#include "RegimentCostsMapper.h"
#include "ParserHelpers.h"

mappers::RegimentCostsMapper::RegimentCostsMapper()
{
	registerKeyword(std::regex("[a-z_]+"), [this](const std::string& regiment, std::istream& theStream)
	{
			commonItems::singleInt costStr(theStream);
			regimentCosts.insert(std::make_pair(regiment, costStr.getInt()));
	});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseFile("regiment_costs.txt");
}
