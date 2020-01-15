#include "NavalBase.h"
#include "ParserHelpers.h"

mappers::NavalBase::NavalBase(std::istream& theStream)
{
	registerKeyword("naval_base", [this](const std::string& unused, std::istream& theStream)
	{
			commonItems::ignoreItem(unused, theStream);
			found = true;
	});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
