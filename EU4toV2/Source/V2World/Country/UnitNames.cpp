#include "UnitNames.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

V2::UnitNames::UnitNames(std::istream& theStream)
{
	registerKeyword("dreadnought", [this](const std::string& unused, std::istream& theStream) {
		dreadnoughts = commonItems::stringList(theStream).getStrings();
	});
	registerKeyword("battleship", [this](const std::string& unused, std::istream& theStream) {
		battleships = commonItems::stringList(theStream).getStrings();
	});
	registerKeyword("ironclad", [this](const std::string& unused, std::istream& theStream) {
		ironclads = commonItems::stringList(theStream).getStrings();
	});
	registerKeyword("manowar", [this](const std::string& unused, std::istream& theStream) {
		manowars = commonItems::stringList(theStream).getStrings();
	});
	registerKeyword("cruiser", [this](const std::string& unused, std::istream& theStream) {
		cruisers = commonItems::stringList(theStream).getStrings();
	});
	registerKeyword("frigate", [this](const std::string& unused, std::istream& theStream) {
		frigates = commonItems::stringList(theStream).getStrings();
	});
	registerKeyword("monitor", [this](const std::string& unused, std::istream& theStream) {
		monitors = commonItems::stringList(theStream).getStrings();
	});
	registerKeyword("clipper_transport", [this](const std::string& unused, std::istream& theStream) {
		clipperTransports = commonItems::stringList(theStream).getStrings();
	});
	registerKeyword("steam_transport", [this](const std::string& unused, std::istream& theStream) {
		steamTransports = commonItems::stringList(theStream).getStrings();
	});
	registerKeyword("commerce_raider", [this](const std::string& unused, std::istream& theStream) {
		commerceRaiders = commonItems::stringList(theStream).getStrings();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}