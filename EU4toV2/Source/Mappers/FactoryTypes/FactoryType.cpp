#include "FactoryType.h"
#include "FactoryTypeInputs.h"
#include "ParserHelpers.h"

mappers::FactoryType::FactoryType(std::istream& theStream, std::string theName): factoryTypeName(std::move(theName))
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::FactoryType::registerKeys()
{
	registerKeyword("output_goods", [this](const std::string& unused, std::istream& theStream) {
		outputs = commonItems::singleString(theStream).getString();
	});
	registerKeyword("is_coastal", [this](const std::string& unused, std::istream& theStream) {
		coastal = commonItems::singleString(theStream).getString() == "yes";
	});
	registerKeyword("input_goods", [this](const std::string& unused, std::istream& theStream) {
		const FactoryTypeInputs v2Inputs(theStream);
		inputs = v2Inputs.getProductionInputValues();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void mappers::FactoryType::assignTechsAndCounts(const std::map<std::string, std::string>& factoryTechMap,
	 const std::map<std::string, std::string>& factoryInventionMap,
	 const std::map<std::string, int>& startingCounts)
{

	if (const auto& factoryTechItr = factoryTechMap.find(factoryTypeName); factoryTechItr != factoryTechMap.end())
		requiredTech = factoryTechItr->second;

	if (const auto& factoryInventionItr = factoryInventionMap.find(factoryTypeName); factoryInventionItr != factoryInventionMap.end())
		requiredInvention = factoryInventionItr->second;

	if (const auto& startingCountItr = startingCounts.find(factoryTypeName); startingCountItr != startingCounts.end())
		startingCount = startingCountItr->second;
}
