#include "CustomPopShaping.h"
#include "../Log.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

mappers::CustomPopShaping::CustomPopShaping(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::CustomPopShaping::loadFile(const std::string& fileName)
{
	registerKeys();
	parseFile(fileName);
	clearRegisteredKeywords();
}

void mappers::CustomPopShaping::registerKeys()
{
	if (theConfiguration.isHpmEnabled())
		registerKeyword("HPM", [this](const std::string& unused, std::istream& theStream) {
			popShapeTypesForRegions(theStream);
		});
	else
		registerKeyword("vanilla", [this](const std::string& unused, std::istream& theStream) {
			popShapeTypesForRegions(theStream);
		});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void mappers::CustomPopShaping::popShapeTypesForRegions(std::istream& theStream)
{
	parser finalParser;
	finalParser.registerKeyword("pop_shaping", [this](const std::string& unused, std::istream& theStream) {
		pop_shaping = commonItems::singleString(theStream).getString();
	});
	finalParser.registerKeyword("provinces", [this](const std::string& unused, std::istream& theStream) {
		std::vector<int> tempVec = commonItems::getInts(theStream);
		std::set<int> tempSet(tempVec.begin(), tempVec.end());
		if (!popShapeTypes.contains(pop_shaping))
			popShapeTypes.emplace(pop_shaping, tempSet);
		else
			for (const auto& i: tempSet)
				popShapeTypes.find(pop_shaping)->second.insert(i);
	});
	finalParser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	parser newParser;
	newParser.registerRegex(commonItems::catchallRegex, [this, &finalParser](const std::string& unused, std::istream& theStream) {
		finalParser.parseStream(theStream);
	});
	newParser.parseStream(theStream);
	newParser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}