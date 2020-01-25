#include "CountryHistoryDate.h"
#include "ParserHelpers.h"

EU4::CountryHistoryDate::CountryHistoryDate(std::istream& theStream, const std::string& leaderClass)
{
	registerKeyword("leader", [this](const std::string& unused, std::istream& theStream)
		{
			const Leader newLeader(theStream);
			leaders.push_back(newLeader);
		});
	registerRegex("monarch|heir|queen", [this](const std::string& leaderClass, std::istream& theStream)
		{
			const CountryHistoryDate lookForLeader(theStream, leaderClass);
			const auto& incLeaders = lookForLeader.getLeaders();
			leaders.insert(leaders.end(), incLeaders.begin(), incLeaders.end());
			if (!lookForLeader.dynasty.empty()) dynasty = lookForLeader.dynasty;
		});
	registerKeyword("dynasty", [this, leaderClass](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString dynastyString(theStream);
			if (leaderClass == "monarch") dynasty = dynastyString.getString();
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
