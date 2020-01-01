#include "EU4Army.h"
#include "ParserHelpers.h"
#include "Log.h"


EU4::EU4Army::EU4Army(std::istream& theStream)
{
	registerKeyword(std::regex("id"), [this](const std::string& unused, std::istream& theStream)
		{
			EU4::EU4UnitID idblock(theStream);
			armyId = idblock;
		});
	registerKeyword(std::regex("leader"), [this](const std::string& unused, std::istream& theStream)
		{
			EU4::EU4UnitID idblock(theStream);
			leaderId = idblock;
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
	registerKeyword(std::regex("[a-z0-9\\_]+"), commonItems::ignoreItem);

	parseStream(theStream);
}

double EU4::EU4Army::getAverageStrength(REGIMENTCATEGORY category) const
{
	int regimentCount = regimentList.size();
	double totalStrength = 0.0;
	for (std::vector<EU4::EU4Regiment>::const_iterator itr = regimentList.begin(); itr != regimentList.end(); ++itr)
	{
		if (itr->getCategory() == category)
		{
			totalStrength += (*itr).getStrength();
		}
	}
	if (regimentCount > 0) return (totalStrength / regimentCount);
	return 0;
}

int EU4::EU4Army::getTotalTypeStrength(REGIMENTCATEGORY category) const
{
	int totalStrength = 0;
	for (std::vector<EU4Regiment>::const_iterator itr = regimentList.begin(); itr != regimentList.end(); ++itr)
	{
		if (itr->getCategory() == category)
		{
			totalStrength += itr->getTypeStrength();
		}
	}
	return totalStrength;
}


void EU4::EU4Army::resolveRegimentTypes(mappers::RegimentTypeMap RTmap)
{
	for (std::vector<EU4Regiment>::iterator itr = regimentList.begin(); itr != regimentList.end(); ++itr)
	{
		try
		{
			itr->setCategory(RTmap[itr->getType()].getCategory());
			itr->setTypeStrength(RTmap[itr->getType()].getStrength());
		}
		catch (std::exception& e)
		{
			std::runtime_error exception("Illegal unit type: " + itr->getType() + ", aborting!");
			throw exception;
		}
	}
}

std::optional<int> EU4::EU4Army::getProbabilisticHomeProvince(EU4::REGIMENTCATEGORY category) const
{
	std::vector<int> homeProvinces;	// the possible home provinces
	for (std::vector<EU4Regiment>::const_iterator itr = regimentList.begin(); itr != regimentList.end(); ++itr)
	{
		if (itr->getCategory() == category)
		{
			const int home = itr->getHome();	// the home of this regiment
			bool blocked = false;					// whether or not this home is blocked
			for (std::vector<int>::const_iterator bitr = blocked_homes.begin(); bitr != blocked_homes.end(); ++bitr)
			{
				if (home == *bitr)
				{
					blocked = true;
					break;
				}
			}
			if (!blocked)
			{
				homeProvinces.push_back(home);
			}
		}
	}
	if (homeProvinces.size() == 0)
	{
		return std::nullopt;
	}

	return homeProvinces[int(homeProvinces.size() * ((double)rand() / RAND_MAX))];
}

void EU4::EU4Army::blockHomeProvince(const int home)
{
	blocked_homes.push_back(home);
}
