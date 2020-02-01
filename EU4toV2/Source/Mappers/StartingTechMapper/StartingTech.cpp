#include "StartingTech.h"
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
	registerKeyword("score", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleDouble scoreDbl(theStream);
			score = scoreDbl.getDouble();
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}