#include "EU4ActiveIdeas.h"
#include "ParserHelpers.h"

EU4::EU4ActiveIdeas::EU4ActiveIdeas(std::istream& theStream)
{
	registerKeyword(std::regex("[a-zA-z0-9_]+"), [this](const std::string& ideaName, std::istream& theStream)
		{
			commonItems::singleInt ideaInt(theStream);
			// Don't bother with incomplete ideas.
			if (ideaInt.getInt() >= 7) activeIdeas.insert(ideaName);
		}
	);
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}


