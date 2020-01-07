#include "EU4Army.h"
#include "ParserHelpers.h"
#include <random>


EU4::EU4Army::EU4Army(std::istream& theStream)
{
	registerKeyword(std::regex("id"), [this](const std::string& unused, std::istream& theStream)
		{
			EU4::ID idblock(theStream);
			armyId = idblock.getIDNum();
		});
	registerKeyword(std::regex("leader"), [this](const std::string& unused, std::istream& theStream)
		{
			EU4::ID idblock(theStream);
			leaderId = idblock.getIDNum();
		});
	registerKeyword(std::regex("name"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString nameStr(theStream);
			name = nameStr.getString();
		});
	registerKeyword(std::regex("regiment|ship"), [this](const std::string& unused, std::istream& theStream)
		{
			EU4Regiment regimentBlock(theStream);
			regimentList.push_back(regimentBlock);
		});
	registerKeyword(std::regex("location"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt locationInt(theStream);
			location = locationInt.getInt();
		});
	// Dropped from saves at 1.20
	registerKeyword(std::regex("at_sea"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt atSeaInt(theStream);
			atSea = atSeaInt.getInt();
		});
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
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


void EU4::EU4Army::resolveRegimentTypes(const mappers::UnitTypeMapper& utm)
{
	auto regimentTypeMap = utm.getUnitTypeMap();
	for (auto& regiment : regimentList)
	{
		try
		{
			regiment.setCategory(regimentTypeMap[regiment.getType()].getCategory());
			regiment.setTypeStrength(regimentTypeMap[regiment.getType()].getStrength());
		}
		catch (std::exception&)
		{
			std::runtime_error exception("Illegal unit type: " + regiment.getType() + ", aborting!");
			throw exception;
		}
	}
}

std::optional<int> EU4::EU4Army::getProbabilisticHomeProvince(EU4::REGIMENTCATEGORY category) const
{
	std::set<int> homeProvinces, randomProvince; // the possible home provinces
	for (const auto& regiment : regimentList)
	{
		if (regiment.getCategory() == category)
		{
			const int home = regiment.getHome();
			if (blocked_homes.count(home)) continue;
			homeProvinces.insert(home);
		}
	}
	if (homeProvinces.size() == 0) return std::nullopt;
	std::sample(homeProvinces.begin(), homeProvinces.end(), std::inserter(randomProvince, randomProvince.begin()), 1, std::mt19937{ std::random_device{}() });
	return *randomProvince.begin();
}

void EU4::EU4Army::blockHomeProvince(const int home)
{
	blocked_homes.insert(home);
}
