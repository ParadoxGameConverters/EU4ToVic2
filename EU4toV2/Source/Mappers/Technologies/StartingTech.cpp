#include "StartingTech.h"
#include "ParserHelpers.h"
#include "Technologies.h"

mappers::StartingTech::StartingTech(std::istream& theStream)
{
	registerKeyword("group", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString groupStr(theStream);
		group = groupStr.getString();
	});
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		name = nameStr.getString();
	});
	registerKeyword("id", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString idStr(theStream);
		id = idStr.getString();
	});
	registerKeyword("score", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleDouble scoreDbl(theStream);
		score = scoreDbl.getDouble();
	});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}

std::string mappers::StartingTech::getName(const std::map<std::string, std::string>& idNameMap) const
{
	std::string returnName;
	if (!name.empty())
		returnName = name;
	else
	{
		returnName = idNameMap.find(id)->second;
	}
	return returnName;
}