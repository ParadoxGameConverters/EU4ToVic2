#include "CountryHistory.h"
#include "CountryHistoryDate.h"
#include "ParserHelpers.h"

EU4::CountryHistory::CountryHistory(std::istream& theStream)
{
	registerKeyword(std::regex("\\d+\\.\\d+\\.\\d+"), [this](const std::string& unused, std::istream& theStream)
		{
			EU4::CountryHistoryDate theDate(theStream);
			auto incLeaders = theDate.getLeaders();
			leaders.insert(leaders.end(), incLeaders.begin(), incLeaders.end());
		}
	);
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
