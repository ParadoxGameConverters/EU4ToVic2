#include "ColonialTag.h"
#include "ParserHelpers.h"


mappers::ColonialTag::ColonialTag(std::istream& theStream)
{
	registerKeyword(std::regex("tag"), [this](const std::string& unused, std::istream& theStream)
	{
		commonItems::singleString tagStr(theStream);
		colonyTag.tag = tagStr.getString();
	});
	registerKeyword(std::regex("EU4_region"), [this](const std::string& unused, std::istream& theStream)
	{
		commonItems::singleString eu4Str(theStream);
		colonyTag.EU4Region = eu4Str.getString();
	});
	registerKeyword(std::regex("V2_region"), [this](const std::string& unused, std::istream& theStream)
	{
		commonItems::singleString v2Str(theStream);
		colonyTag.V2Region = v2Str.getString();
	});
	registerKeyword(std::regex("is_culture_group"), [this](const std::string& unused, std::istream& theStream)
	{
		commonItems::singleString cgStr(theStream);
		colonyTag.cultureGroup = cgStr.getString();
	});

	parseStream(theStream);
}
