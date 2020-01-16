#include "MapperPop.h"
#include "ParserHelpers.h"

mappers::MapperPop::MapperPop(std::istream& theStream)
{
	registerKeyword("culture", [this](const std::string& unused, std::istream& theStream)
	{
			commonItems::singleString cultureStr(theStream);
			culture = cultureStr.getString();
	});
	registerKeyword("religion", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString religionStr(theStream);
			religion = religionStr.getString();
		});
	registerKeyword("size", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt sizeInt(theStream);
			size = sizeInt.getInt();
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
