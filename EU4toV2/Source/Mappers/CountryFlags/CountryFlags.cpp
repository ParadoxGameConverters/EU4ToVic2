#include "CountryFlags.h"
#include "CountryFlagsMapping.h"
#include "ParserHelpers.h"

mappers::CountryFlags::CountryFlags()
{
	registerKeys();
	parseFile("configurables/flag_mappings.txt");
	clearRegisteredKeywords();
};

void mappers::CountryFlags::registerKeys()
{
	registerKeyword("link", [this](const std::string& unused, std::istream& theStream) {
		const CountryFlagsMapping newMapping(theStream);
		if (!newMapping.getEU4Flag().empty())
			flags.insert(std::make_pair(newMapping.getEU4Flag(), newMapping.getV2Flags()));
	});
}