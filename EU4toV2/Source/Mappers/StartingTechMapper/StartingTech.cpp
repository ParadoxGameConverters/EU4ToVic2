#include "StartingTech.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

mappers::StartingTech::StartingTech(std::istream& theStream)
{
	registerKeyword("group", [this](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleString groupStr(theStream);
			group = groupStr.getString();
		});
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString nameStr(theStream);
			name = nameStr.getString();
		});
	registerKeyword("hpm", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString hpmStr(theStream);
			hpm = hpmStr.getString();
		});
	registerKeyword("score", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleDouble scoreDbl(theStream);
			score = scoreDbl.getDouble();
		});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}