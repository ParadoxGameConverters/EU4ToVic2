#include "EU4WarDetails.h"
#include "ParserHelpers.h"

EU4::WarDetails::WarDetails(std::istream& theStream)
{
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
