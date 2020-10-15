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
	registerKeyword("aiWillDo", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::stringOfItem aiWillDoStr(theStream);
		aiWillDo = aiWillDoStr.getString();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}