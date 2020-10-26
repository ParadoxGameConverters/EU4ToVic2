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
		vanillaTag = commonItems::singleString(theStream).getString();
	});
	registerKeyword("mod", [this](const std::string& unused, std::istream& theStream) {
		modTag = commonItems::singleString(theStream).getString();
	});
	registerKeyword("old", [this](const std::string& unused, std::istream& theStream) {
		oldTag = commonItems::singleString(theStream).getString();
	});
	registerKeyword("new", [this](const std::string& unused, std::istream& theStream) {
		newTag = commonItems::singleString(theStream).getString();
	});
	registerKeyword("reform", [this](const std::string& unused, std::istream& theStream) {
		reforms.insert(commonItems::singleString(theStream).getString());
	});
	registerKeyword("flag", [this](const std::string& unused, std::istream& theStream) {
		flags.insert(commonItems::singleString(theStream).getString());
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
