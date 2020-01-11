#include "EU4GovernmentSection.h"
#include "ParserHelpers.h"

EU4::GovernmentSection::GovernmentSection(std::istream& theStream)
{
	registerKeyword("government", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString governmentStr(theStream);
			government = governmentStr.getString();
		});
	registerKeyword("reform_stack", [this](const std::string& unused, std::istream& theStream)
		{
			EU4::ReformStackSection refStack(theStream);
			reformStack = refStack.getReforms();
		});
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}