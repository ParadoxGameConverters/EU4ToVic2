#include "ColonialTag.h"
#include "ParserHelpers.h"

mappers::ColonialTag::ColonialTag(std::istream& theStream)
{
	registerKeyword("tag", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString tagStr(theStream);
			colonyTag.tag = tagStr.getString();
		});
	registerKeyword("EU4_region", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString eu4Str(theStream);
			colonyTag.EU4Region = eu4Str.getString();
		});
	registerKeyword("V2_region", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString v2Str(theStream);
			colonyTag.V2Region = v2Str.getString();
		});
	registerKeyword("is_culture_group", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString cgStr(theStream);
			colonyTag.cultureGroup = cgStr.getString();
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
