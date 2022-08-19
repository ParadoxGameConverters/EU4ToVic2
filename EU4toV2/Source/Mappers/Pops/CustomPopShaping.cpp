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

	std::string pop_shaping;
	std::set<int> provinces;

	finalParser.registerKeyword("pop_shaping", [this, &pop_shaping](const std::string& unused, std::istream& theStream) {
		pop_shaping = commonItems::singleString(theStream).getString();
	});
	finalParser.registerKeyword("provinces", [this, &provinces](const std::string& unused, std::istream& theStream) {
		std::vector<int> tempVec = commonItems::getInts(theStream);
		provinces.insert(tempVec.begin(), tempVec.end());
	});
	finalParser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	parser newParser;
	newParser.registerRegex(commonItems::catchallRegex, [this, &finalParser, &pop_shaping, &provinces](const std::string& unused, std::istream& theStream) {
		finalParser.parseStream(theStream);
		if (!popShapeTypes.contains(pop_shaping))
			popShapeTypes.emplace(pop_shaping, provinces);
		else
			for (const auto& i: provinces)
				popShapeTypes.find(pop_shaping)->second.insert(i);
	});
	newParser.parseStream(theStream);
	newParser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}