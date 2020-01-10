#include "ID.h"
#include "ParserHelpers.h"

EU4::ID::ID(std::istream& theStream)
{
	registerKeyword(std::regex("id"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt theNum(theStream);
			IDNum = theNum.getInt();
		});
	registerKeyword(std::regex("type"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt theNum(theStream);
			IDType = theNum.getInt();
		});
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), commonItems::ignoreItem);

	parseStream(theStream);
}