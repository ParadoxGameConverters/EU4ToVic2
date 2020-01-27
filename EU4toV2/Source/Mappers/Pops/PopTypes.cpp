#include "PopTypes.h"
#include "ParserHelpers.h"

mappers::PopTypes::PopTypes(std::istream& theStream)
{
	registerRegex("[a-z]+", [this](const std::string& popType, std::istream& theStream)
		{
			MapperPop newPop(theStream);
			popTypes.insert(std::make_pair(popType, newPop));
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
