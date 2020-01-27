#include "CountryHistory.h"
#include "CountryHistoryDate.h"
#include "ParserHelpers.h"

EU4::CountryHistory::CountryHistory(std::istream& theStream)
{
	registerRegex("\\d+\\.\\d+\\.\\d+", [this](const std::string& unused, std::istream& theStream)
		{
			const CountryHistoryDate theDate(theStream, std::string());
			auto incLeaders = theDate.getLeaders();
			leaders.insert(leaders.end(), incLeaders.begin(), incLeaders.end());
			if (!theDate.getDynasty().empty()) dynasty = theDate.getDynasty();
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
