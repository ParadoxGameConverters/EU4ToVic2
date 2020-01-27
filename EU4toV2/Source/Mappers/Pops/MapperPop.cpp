#include "MapperPop.h"
#include "ParserHelpers.h"

mappers::MapperPop::MapperPop(std::istream& theStream)
{
	registerKeyword("culture", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString cultureStr(theStream);
			culture = cultureStr.getString();
		});
	registerKeyword("religion", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString religionStr(theStream);
			religion = religionStr.getString();
		});
	registerKeyword("size", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleDouble sizeDbl(theStream);
			size = static_cast<int>(sizeDbl.getDouble());
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
