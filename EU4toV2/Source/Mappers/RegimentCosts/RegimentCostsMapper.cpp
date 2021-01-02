#include "RegimentCostsMapper.h"
#include "Log.h"
#include "ParserHelpers.h"

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
	registerRegex("[a-z_]+", [this](const std::string& regiment, std::istream& theStream) {
		const commonItems::singleInt costStr(theStream);
		regimentCosts.insert(std::make_pair(regiment, costStr.getInt()));
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

std::optional<int> mappers::RegimentCostsMapper::getCostForRegimentType(const std::string& regimentType) const
{
	if (const auto& costs = regimentCosts.find(regimentType); costs != regimentCosts.end())
		return costs->second;
	else
		return std::nullopt;
}
