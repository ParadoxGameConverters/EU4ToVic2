#include "V2Unreleasables.h"
#include "ParserHelpers.h"


mappers::V2Unreleasables::V2Unreleasables()
{
	registerKeyword(std::regex("unreleasable"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString unreleasableStr(theStream);
			unreleasables.insert(unreleasableStr.getString());
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseFile("unreleasable_tags.txt");
}
