#include "StartingTech.h"
#include "CommonRegexes.h"
#include "Configuration.h"
#include "ParserHelpers.h"

mappers::StartingTech::StartingTech(std::istream& theStream)
{
	registerKeyword("group", [this](const std::string& unused, std::istream& theStream) {
		group = commonItems::singleString(theStream).getString();
	});
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		name = commonItems::singleString(theStream).getString();
	});
	registerKeyword("hpm", [this](const std::string& unused, std::istream& theStream) {
		hpm = commonItems::singleString(theStream).getString();
	});
	registerKeyword("score", [this](const std::string& unused, std::istream& theStream) {
		score = commonItems::singleDouble(theStream).getDouble();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();

	if (theConfiguration.isHpmEnabled() && !hpm.empty())
	{
		name = hpm;
	}
}