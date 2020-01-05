#include "NavalBase.h"
#include "ParserHelpers.h"

mappers::NavalBase::NavalBase(std::istream& theStream)
{
	registerKeyword(std::regex("naval_base"), [this](const std::string& unused, std::istream& theStream)
	{
			commonItems::ignoreItem(unused, theStream);
			found = true;
	});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}
