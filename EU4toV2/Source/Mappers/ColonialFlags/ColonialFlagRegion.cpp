#include "ColonialFlagRegion.h"
#include "ParserHelpers.h"

mappers::ColonialFlagRegion::ColonialFlagRegion(std::istream& theStream, const std::string& region)
{
	registerKeyword("flag", [this, region](const std::string& unused, std::istream& theStream)
		{
			ColonialFlag newFlag(theStream, region);
			regionalFlags.insert(std::make_pair(newFlag.getName(), newFlag));
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
