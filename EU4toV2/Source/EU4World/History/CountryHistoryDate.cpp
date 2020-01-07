#include "CountryHistoryDate.h"
#include "ParserHelpers.h"

EU4::CountryHistoryDate::CountryHistoryDate(std::istream& theStream)
{
	registerKeyword(std::regex("leader"), [this](const std::string& unused, std::istream& theStream)
		{
			EU4::Leader newLeader(theStream);
			leaders.push_back(newLeader);
		}
	);
	registerKeyword(std::regex("monarch|heir|queen"), [this](const std::string& unused, std::istream& theStream)
		{
			EU4::CountryHistoryDate lookForLeader(theStream);
			auto incLeaders = lookForLeader.getLeaders();
			leaders.insert(leaders.end(), incLeaders.begin(), incLeaders.end());
		}
	);
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}
