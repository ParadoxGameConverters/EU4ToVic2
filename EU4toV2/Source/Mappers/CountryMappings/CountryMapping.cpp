#include "CountryMapping.h"
#include "ParserHelpers.h"
#include "CommonRegexes.h"

mappers::CountryMapping::CountryMapping(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::CountryMapping::registerKeys()
{
	registerKeyword("eu4", [this](const std::string& unused, std::istream& theStream) {
		eu4Tag = commonItems::singleString(theStream).getString();
	});
	registerKeyword("v2", [this](const std::string& unused, std::istream& theStream) {
		vic2Tag = commonItems::singleString(theStream).getString();
	});
	registerKeyword("vic", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString eu4str(theStream);
		vanillaTag = eu4str.getString();
		std::transform(vanillaTag.begin(), vanillaTag.end(), vanillaTag.begin(), ::toupper);
	});
	registerKeyword("mod", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString v2str(theStream);
		modTag = v2str.getString();
		std::transform(modTag.begin(), modTag.end(), modTag.begin(), ::toupper);
	});
	registerKeyword("reform", [this](const std::string& unused, std::istream& theStream) {
		reforms.insert(commonItems::singleString(theStream).getString());
	});
	registerKeyword("flag", [this](const std::string& unused, std::istream& theStream) {
		flags.insert(commonItems::singleString(theStream).getString());
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
