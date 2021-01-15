#include "EU4WarDetails.h"
#include "ParserHelpers.h"
#include "CommonRegexes.h"

void EU4::WarDetails::addDetails(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU4::WarDetails::registerKeys()
{
	registerKeyword("type", [this](const std::string& unused, std::istream& theStream) {
		warGoalType = commonItems::singleString(theStream).getString();
	});
	registerKeyword("province", [this](const std::string& unused, std::istream& theStream) {
		targetProvinceID = commonItems::singleInt(theStream).getInt();
	});
	registerKeyword("tag", [this](const std::string& unused, std::istream& theStream) {
		targetTag = commonItems::singleString(theStream).getString();
	});
	registerRegex(R"(\d+\.\d+\.\d+)", [this](const std::string& dateString, std::istream& theStream) {
		if (!startDate.isSet())
			startDate = date(dateString);
		commonItems::ignoreItem(dateString, theStream);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
