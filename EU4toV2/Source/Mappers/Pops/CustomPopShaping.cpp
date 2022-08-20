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

	ShapingType shapingType;
	std::set<int> provinces;

	finalParser.registerKeyword("pop_shaping", [this, &shapingType](const std::string& unused, std::istream& theStream) {
		const auto type = commonItems::singleString(theStream).getString();
		if (type == "vanilla")
			shapingType = ShapingType::vanilla_type;
		else if (type == "devPush")
			shapingType = ShapingType::dev_push_type;
		else if (type == "absolute")
			shapingType = ShapingType::absolute_type;
		else
		{
			shapingType = ShapingType::vanilla_type;
			LOG(LogLevel::Warning) << "Custom pop_shaping " << type << "is not a valid type, switching to Vanilla";
		}
	});
	finalParser.registerKeyword("provinces", [this, &provinces](const std::string& unused, std::istream& theStream) {
		std::vector<int> tempVec = commonItems::getInts(theStream);
		provinces.insert(tempVec.begin(), tempVec.end());
	});
	finalParser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	parser newParser;
	newParser.registerRegex(commonItems::catchallRegex, [this, &finalParser, &shapingType, &provinces](const std::string& unused, std::istream& theStream) {
		finalParser.parseStream(theStream);
		for (const auto& province: provinces)
		{
			popShapeTypes.emplace(province, shapingType);
		}
	});
	newParser.parseStream(theStream);
	newParser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}


mappers::ShapingType mappers::CustomPopShaping::getPopShapeType(int provinceID) const
{
	if (const auto type = popShapeTypes.find(provinceID); type != popShapeTypes.end())
	{
		return type->second;
	}

	LOG(LogLevel::Warning) << "Custom pop_shaping for Province " << provinceID << " is not set correctly, switching to Vanilla";
	return ShapingType::vanilla_type;
}
