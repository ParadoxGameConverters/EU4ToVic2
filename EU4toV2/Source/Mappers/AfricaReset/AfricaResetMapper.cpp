#include "AfricaResetMapper.h"
#include "ParserHelpers.h"
#include "Log.h"

mappers::AfricaResetMapper::AfricaResetMapper()
{
	LOG(LogLevel::Info) << "Parsing african resettable regions.";
	registerKeyword("reset_regions", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::stringList theRegionsList(theStream);
		auto theRegions = theRegionsList.getStrings();
		resettableRegions = std::set(theRegions.begin(), theRegions.end());
	});
	registerKeyword("reset_below_westernization", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt resetInt(theStream);
		reset_below_westernization = resetInt.getInt();
	});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseFile("configurables/africa_reset.txt");
	clearRegisteredKeywords();
}
