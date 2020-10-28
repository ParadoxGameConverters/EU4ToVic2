#include "CountryFlagsMapping.h"
#include "ParserHelpers.h"

mappers::CountryFlagsMapping::CountryFlagsMapping(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::CountryFlagsMapping::registerKeys()
{
	registerKeyword("eu4", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString _eu4Flag(theStream);
		eu4Flag = _eu4Flag.getString();
	});
	registerKeyword("v2", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString _v2Flag(theStream);
		v2Flags.push_back(_v2Flag.getString());
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}