#include "PopTypes.h"
#include "ParserHelpers.h"

mappers::PopTypes::PopTypes(std::istream& theStream)
{
	registerKeyword(std::regex("[a-z]+"), [this](const std::string& popType, std::istream& theStream)
		{
			mappers::Pop newPop(theStream);
			popTypes.insert(std::make_pair(popType, newPop));
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}
