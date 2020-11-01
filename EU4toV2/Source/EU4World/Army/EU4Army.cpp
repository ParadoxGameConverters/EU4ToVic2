#include "EU4Army.h"
#include "ParserHelpers.h"

EU4::EU4Army::EU4Army(std::istream& theStream, const std::string& potentialNavy)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();

	if (potentialNavy == "navy")
		armyFloats = true;
}

void EU4::EU4Army::registerKeys()
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		name = commonItems::singleString(theStream).getString();
	});
	registerRegex("regiment|ship", [this](const std::string& unused, std::istream& theStream) {
		const EU4Regiment regimentBlock(theStream);
		regimentList.push_back(regimentBlock);
	});
	registerKeyword("location", [this](const std::string& unused, std::istream& theStream) {
		location = commonItems::singleInt(theStream).getInt();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

double EU4::EU4Army::getAverageStrength(const REGIMENTCATEGORY category) const
{
	const auto regimentCount = static_cast<double>(regimentList.size());
	auto totalStrength = 0.0;
	for (const auto& regiment: regimentList)
		if (regiment.getCategory() == category)
			totalStrength += regiment.getStrength();

	if (regimentCount > 0)
		return totalStrength / regimentCount;
	else
		return 0.0;
}

int EU4::EU4Army::getTotalTypeStrength(const REGIMENTCATEGORY category) const
{
	auto totalStrength = 0;
	for (const auto& regiment: regimentList)
		if (regiment.getCategory() == category)
			totalStrength += regiment.getTypeStrength();

	return totalStrength;
}

void EU4::EU4Army::resolveRegimentTypes(const mappers::UnitTypeMapper& unitTypeMapper)
{
	auto regimentTypeMap = unitTypeMapper.getUnitTypeMap();
	for (auto& regiment: regimentList)
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
