#include "FactoryTypeInputs.h"
#include "ParserHelpers.h"

mappers::FactoryTypeInputs::FactoryTypeInputs(std::istream& theStream)
{
	registerRegex("[a-z_]+", [this](const std::string& inputGood, std::istream& theStream)
		{
			commonItems::singleDouble inputValue(theStream);
			productionInputValues.insert(std::make_pair(inputGood, inputValue.getDouble()));
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
