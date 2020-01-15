#include "FactoryType.h"
#include "FactoryTypeInputs.h"
#include "ParserHelpers.h"

mappers::FactoryType::FactoryType(std::istream& theStream)
{
	registerKeyword("output_goods", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString outputStr(theStream);
			outputs = outputStr.getString();
		});
	registerKeyword("is_coastal", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString coastalStr(theStream);
			coastal = (coastalStr.getString() == "yes");
		});
	registerKeyword("input_goods", [this](const std::string& unused, std::istream& theStream)
		{
			FactoryTypeInputs v2Inputs(theStream);
			inputs = v2Inputs.getProductionInputValues();
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::FactoryType::assignTechsAndCounts(
	const std::map<std::string, std::string>& factoryTechMap,
	const std::map<std::string, std::string>& factoryInventionMap,
	const std::map<std::string, int>& startingCounts)
{
	const auto& factoryTechItr = factoryTechMap.find(factoryTypeName);
	if (factoryTechItr != factoryTechMap.end())
	{
		requiredTech = factoryTechItr->second;
	}
	const auto& factoryInventionItr = factoryInventionMap.find(factoryTypeName);
	if (factoryInventionItr != factoryInventionMap.end())
	{
		requiredInvention = factoryInventionItr->second;
	}
	const auto& startingCountItr = startingCounts.find(factoryTypeName);
	if (startingCountItr != startingCounts.end())
	{
		startingCount = startingCountItr->second;
	}
}
