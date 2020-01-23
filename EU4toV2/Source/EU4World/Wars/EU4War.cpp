#include "EU4War.h"
#include "EU4WarDetails.h"
#include "ParserHelpers.h"

EU4::War::War(std::istream& theStream)
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString nameStr(theStream);
			name = nameStr.getString();
		});
	registerKeyword("history", [this](const std::string& unused, std::istream& theStream)
		{
			WarDetails history(theStream);
			startDate = history.getStartDate();
		});
	registerKeyword("attackers", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::stringList attackersStr(theStream);
			attackers = attackersStr.getStrings();
		});
	registerKeyword("defenders", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::stringList defendersStr(theStream);
			defenders = defendersStr.getStrings();
		});
	registerRegex("take_|defend_", [this](const std::string& unused, std::istream& theStream)
		{
			WarDetails details(theStream);
			warType = details.getWarType();
			targetProvince = details.getTargetProvince();
			casusBelli = details.getCassusBelli();
		});

	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
