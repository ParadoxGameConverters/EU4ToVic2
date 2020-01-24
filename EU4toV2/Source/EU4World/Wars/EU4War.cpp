#include "EU4War.h"
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
			details.addDetails(theStream);
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
	registerRegex("take_\\w+|defend_\\w+|\\w*superiority|blockade_\\w+", [this](const std::string& warGoalClass, std::istream& theStream)
		{
			details.addDetails(theStream);
			details.warGoalClass = warGoalClass;
		});

	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
