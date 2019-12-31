#include "MinorityPop.h"
#include "ParserHelpers.h"


mappers::MinorityPop::MinorityPop(std::istream& theStream)
{
	registerKeyword(std::regex("religion"), [this](const std::string& unused, std::istream& theStream)
	{
		commonItems::singleString religionStr(theStream);
		religion = religionStr.getString();
	});
	registerKeyword(std::regex("culture"), [this](const std::string& unused, std::istream& theStream)
	{
		commonItems::singleString cultureStr(theStream);
		culture = cultureStr.getString();
	});
	registerKeyword(std::regex("[a-z0-9\\_]+"), commonItems::ignoreItem);

	parseStream(theStream);
}
