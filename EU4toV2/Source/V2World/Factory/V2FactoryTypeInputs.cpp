#include "V2FactoryTypeInputs.h"
#include "ParserHelpers.h"


mappers::V2FactoryTypeInputs::V2FactoryTypeInputs(std::istream& theStream)
{
	registerKeyword(std::regex("[a-z_]+"), [this](const std::string& inputGood, std::istream& theStream)
		{
			commonItems::singleDouble inputValue(theStream);
			productionInputValues.insert(std::make_pair(inputGood, inputValue.getDouble()));
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_]+"), commonItems::ignoreItem);

	parseStream(theStream);
}
