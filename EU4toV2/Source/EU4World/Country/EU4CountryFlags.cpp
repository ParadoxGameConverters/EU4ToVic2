#include "EU4CountryFlags.h"
#include "ParserHelpers.h"

EU4::EU4CountryFlags::EU4CountryFlags(std::istream& theStream)
{
	registerKeyword(std::regex("[a-z_]+"), [this](const std::string& flag, std::istream& theStream)
		{
			commonItems::ignoreItem(flag, theStream);
			flags.insert(flag);
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);
	parseStream(theStream);
}
