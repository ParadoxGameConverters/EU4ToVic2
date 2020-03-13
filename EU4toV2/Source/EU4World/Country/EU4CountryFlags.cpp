#include "EU4CountryFlags.h"
#include "ParserHelpers.h"

EU4::EU4CountryFlags::EU4CountryFlags(std::istream& theStream)
{
	registerRegex("[a-zA-Z0-9_]+", [this](const std::string& flag, std::istream& theStream)
		{
			commonItems::ignoreItem(flag, theStream);
			flags.insert(flag);
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
