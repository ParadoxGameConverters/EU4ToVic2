#include "V2FactoryType.h"
#include "V2FactoryTypeInputs.h"
#include "ParserHelpers.h"


mappers::V2FactoryType::V2FactoryType(std::istream& theStream)
{
	registerKeyword(std::regex("output_goods"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString outputStr(theStream);
			outputs = outputStr.getString();
		});
	registerKeyword(std::regex("is_coastal"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString coastalStr(theStream);
			coastal = (coastalStr.getString() == "yes");
		});
	registerKeyword(std::regex("input_goods"), [this](const std::string& unused, std::istream& theStream)
		{
			V2FactoryTypeInputs v2Inputs(theStream);
			inputs = v2Inputs.getProductionInputValues();
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_]+"), commonItems::ignoreItem);

	parseStream(theStream);
}

void mappers::V2FactoryType::assignTechsAndCounts(
	const std::map<std::string, std::string>& factoryTechMap,
	const std::map<std::string, std::string>& factoryInventionMap,
	const std::map<std::string, int>& startingCounts
)
{
	std::map<std::string, std::string>::const_iterator factoryTechItr = factoryTechMap.find(factoryTypeName);
	if (factoryTechItr != factoryTechMap.end())
	{
		requiredTech = factoryTechItr->second;
	}
	std::map<std::string, std::string>::const_iterator factoryInventionItr = factoryInventionMap.find(factoryTypeName);
	if (factoryInventionItr != factoryInventionMap.end())
	{
		requiredInvention = factoryInventionItr->second;
	}
	std::map<std::string, int>::const_iterator startingCountItr = startingCounts.find(factoryTypeName);
	if (startingCountItr != startingCounts.end())
	{
		startingCount = startingCountItr->second;
	}
}
