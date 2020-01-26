#include "UnitType.h"
#include "ParserHelpers.h"

mappers::UnitType::UnitType(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

mappers::UnitType::UnitType(const std::string& filePath)
{
	registerKeys();
	parseFile(filePath);
	clearRegisteredKeywords();
}

void mappers::UnitType::registerKeys()
{
	registerRegex("maneuver|offensive_morale|defensive_morale|offensive_fire|defensive_fire|offensive_shock|defensive_shock|hull_size",
		[this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleInt strInt(theStream);
			strength += strInt.getInt();
		});
	registerKeyword("type", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString typeStr(theStream);
			const auto& checkType = EU4::RegimentCategoryNames.find(typeStr.getString());
			if (checkType == EU4::RegimentCategoryNames.end())
				throw std::runtime_error("Illegal unit type: " + typeStr.getString() + ", aborting!");
			unitType = EU4::RegimentCategoryNames[typeStr.getString()];
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}