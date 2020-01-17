#include "EU4Army.h"
#include "ParserHelpers.h"
#include "../ID.h"

EU4::EU4Army::EU4Army(std::istream& theStream, const std::string& _armyFloats)
{
	registerKeyword("id", [this](const std::string& unused, std::istream& theStream)
		{
			EU4::ID idblock(theStream);
			armyId = idblock.getIDNum();
		});
	registerKeyword("leader", [this](const std::string& unused, std::istream& theStream)
		{
			EU4::ID idblock(theStream);
			leaderId = idblock.getIDNum();
		});
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString nameStr(theStream);
			name = nameStr.getString();
		});
	registerRegex("regiment|ship", [this](const std::string& unused, std::istream& theStream)
		{
			EU4Regiment regimentBlock(theStream);
			regimentList.push_back(regimentBlock);
		});
	registerKeyword("location", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt locationInt(theStream);
			location = locationInt.getInt();
		});
	// Dropped from saves at 1.20
	registerKeyword("at_sea", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt atSeaInt(theStream);
			atSea = atSeaInt.getInt();
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();

	if (_armyFloats == "navy") armyFloats = true;
}

double EU4::EU4Army::getAverageStrength(REGIMENTCATEGORY category) const
{
	int regimentCount = regimentList.size();
	double totalStrength = 0.0;
	for (const auto& regiment : regimentList)
	{
		if (regiment.getCategory() == category)
		{
			totalStrength += regiment.getStrength();
		}
	}
	if (regimentCount > 0) return (totalStrength / regimentCount);
	return 0;
}

int EU4::EU4Army::getTotalTypeStrength(REGIMENTCATEGORY category) const
{
	int totalStrength = 0;
	for (const auto& regiment : regimentList)
	{
		if (regiment.getCategory() == category)
		{
			totalStrength += regiment.getTypeStrength();
		}
	}
	return totalStrength;
}

void EU4::EU4Army::resolveRegimentTypes(const mappers::UnitTypeMapper& unitTypeMapper)
{
	auto regimentTypeMap = unitTypeMapper.getUnitTypeMap();
	for (auto& regiment : regimentList)
	{
		try
		{
			regiment.setCategory(regimentTypeMap[regiment.getType()].getCategory());
			regiment.setTypeStrength(regimentTypeMap[regiment.getType()].getStrength());
		}
		catch (std::exception&)
		{			
			throw std::runtime_error("Illegal unit type: " + regiment.getType() + ", aborting!");
		}
	}
}
