#include "FactoryTypeInputs.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

mappers::FactoryTypeInputs::FactoryTypeInputs(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::FactoryTypeInputs::registerKeys()
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& inputGood, std::istream& theStream) {
		productionInputValues.insert(std::make_pair(inputGood, commonItems::singleDouble(theStream).getDouble()));
	});
}
