#include "Technology.h"
#include "CommonRegexes.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

mappers::Technology::Technology(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::Technology::registerKeys()
{
	registerKeyword("area", [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleString areaStr(theStream);
		area = areaStr.getString();
	});
	registerKeyword("year", [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt yearStr(theStream);
		year = yearStr.getInt();
	});
	registerKeyword("cost", [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt costStr(theStream);
		cost = costStr.getInt();
	});
	registerKeyword("unciv_military", [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleString uncivStr(theStream);
		uncivMilitary = uncivStr.getString();
	});
	registerKeyword("ai_chance", [this](const std::string& unused, std::istream& theStream) {
		commonItems::stringOfItem aiChanceStr(theStream);
		aiChance = aiChanceStr.getString();
	});
	registerRegex("[a-z_]+", [this](const std::string& theEffect, std::istream& theStream) {
		commonItems::stringOfItem effectsStr(theStream);
		effects.push_back(theEffect + " " + effectsStr.getString());
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}