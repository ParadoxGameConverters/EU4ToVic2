#include "MinorityPop.h"
#include "ParserHelpers.h"

mappers::MinorityPop::MinorityPop(std::istream& theStream)
{
	registerKeyword("religion", [this](const std::string& unused, std::istream& theStream)
	{
		commonItems::singleString religionStr(theStream);
		religion = religionStr.getString();
	});
	registerKeyword("culture", [this](const std::string& unused, std::istream& theStream)
	{
		commonItems::singleString cultureStr(theStream);
		culture = cultureStr.getString();
	});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
