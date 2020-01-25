#include "EU4Army.h"
#include "ParserHelpers.h"

EU4::EU4Army::EU4Army(std::istream& theStream, const std::string& potentialNavy)
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString nameStr(theStream);
			name = nameStr.getString();
		});
	registerRegex("regiment|ship", [this](const std::string& unused, std::istream& theStream)
		{
			const EU4Regiment regimentBlock(theStream);
			regimentList.push_back(regimentBlock);
		});
	registerKeyword("location", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleInt locationInt(theStream);
			location = locationInt.getInt();
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();

	if (potentialNavy == "navy") armyFloats = true;
}

double EU4::EU4Army::getAverageStrength(REGIMENTCATEGORY category) const
{
	const int regimentCount = regimentList.size();
	auto totalStrength = 0.0;
	for (const auto& regiment : regimentList)
	{
		if (regiment.getCategory() == category)
		{
			totalStrength += regiment.getStrength();
		}
	}
	if (regimentCount) return totalStrength / regimentCount;
	return 0;
}

int EU4::EU4Army::getTotalTypeStrength(REGIMENTCATEGORY category) const
{
	auto totalStrength = 0;
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
