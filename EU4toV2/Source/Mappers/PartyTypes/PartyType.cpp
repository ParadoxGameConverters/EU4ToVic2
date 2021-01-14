#include "PartyType.h"
#include "ParserHelpers.h"
#include "CommonRegexes.h"

mappers::PartyType::PartyType(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::PartyType::registerKeys()
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		name = commonItems::singleString(theStream).getString();
	});
	registerKeyword("ideology", [this](const std::string& unused, std::istream& theStream) {
		ideology = commonItems::singleString(theStream).getString();
	});
	registerKeyword("start_date", [this](const std::string& unused, std::istream& theStream) {
		start_date = date(commonItems::singleString(theStream).getString());
	});
	registerKeyword("end_date", [this](const std::string& unused, std::istream& theStream) {
		end_date = date(commonItems::singleString(theStream).getString());
	});
	registerKeyword("economic_policy", [this](const std::string& unused, std::istream& theStream) {
		economic_policy = commonItems::singleString(theStream).getString();
	});
	registerKeyword("trade_policy", [this](const std::string& unused, std::istream& theStream) {
		trade_policy = commonItems::singleString(theStream).getString();
	});
	registerKeyword("religious_policy", [this](const std::string& unused, std::istream& theStream) {
		religious_policy = commonItems::singleString(theStream).getString();
	});
	registerKeyword("citizenship_policy", [this](const std::string& unused, std::istream& theStream) {
		citizenship_policy = commonItems::singleString(theStream).getString();
	});
	registerKeyword("war_policy", [this](const std::string& unused, std::istream& theStream) {
		war_policy = commonItems::singleString(theStream).getString();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
