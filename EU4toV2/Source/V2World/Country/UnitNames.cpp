#include "UnitNames.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

V2::UnitNames::UnitNames(std::istream& theStream)
{
	registerKeyword("dreadnought", [this](const std::string& unused, std::istream& theStream) {
		dreadnoughts = commonItems::getStrings(theStream);
	});
	registerKeyword("battleship", [this](const std::string& unused, std::istream& theStream) {
		battleships = commonItems::getStrings(theStream);
	});
	registerKeyword("ironclad", [this](const std::string& unused, std::istream& theStream) {
		ironclads = commonItems::getStrings(theStream);
	});
	registerKeyword("manowar", [this](const std::string& unused, std::istream& theStream) {
		manowars = commonItems::getStrings(theStream);
	});
	registerKeyword("cruiser", [this](const std::string& unused, std::istream& theStream) {
		cruisers = commonItems::getStrings(theStream);
	});
	registerKeyword("frigate", [this](const std::string& unused, std::istream& theStream) {
		frigates = commonItems::getStrings(theStream);
	});
	registerKeyword("monitor", [this](const std::string& unused, std::istream& theStream) {
		monitors = commonItems::getStrings(theStream);
	});
	registerKeyword("clipper_transport", [this](const std::string& unused, std::istream& theStream) {
		clipperTransports = commonItems::getStrings(theStream);
	});
	registerKeyword("steam_transport", [this](const std::string& unused, std::istream& theStream) {
		steamTransports = commonItems::getStrings(theStream);
	});
	registerKeyword("commerce_raider", [this](const std::string& unused, std::istream& theStream) {
		commerceRaiders = commonItems::getStrings(theStream);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}