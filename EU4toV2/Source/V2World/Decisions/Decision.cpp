#include "Decision.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

V2::Decision::Decision(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void V2::Decision::registerKeys()
{
	registerRegex("picture|image", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString pictureStr(theStream);
		picture = pictureStr.getString();
	});
	registerKeyword("news", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString newsStr(theStream);
		news = newsStr.getString();
	});
	registerKeyword("potential", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::stringOfItem potentialStr(theStream);
		potential = potentialStr.getString();
	});
	registerKeyword("allow", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::stringOfItem allowStr(theStream);
		allow = allowStr.getString();
	});
	registerKeyword("effect", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::stringOfItem effectStr(theStream);
		effect = effectStr.getString();
	});
	registerKeyword("ai_will_do", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::stringOfItem aiWillDoStr(theStream);
		aiWillDo = aiWillDoStr.getString();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void V2::Decision::updateDecision(const std::string& section, const std::string& newText)
{
	if (section == "potential") potential = newText;
	if (section == "allow") allow = newText;
	if (section == "effect") effect = newText;
	if (section == "aiWillDo") aiWillDo = newText;
}