#include "Army.h"
#include "Log.h"
#include <algorithm>
#include <random>
#include <queue>
#include "CardinalToOrdinal.h"

V2::Army::Army(const EU4::EU4Army& eu4Army,
               const std::string& _tag,
               bool civilized, 
               const mappers::RegimentCostsMapper& regimentCostsMapper, 
               std::map<int, V2Province*> allProvinces,
               const mappers::ProvinceMapper& provinceMapper,
               const mappers::PortProvinces& portProvincesMapper, 
					std::map<REGIMENTTYPE, int>& unitNameCount,
					const std::string& localAdjective):
	name(eu4Army.getName()), tag(_tag)
{
	// See what we're dealing with
	for (auto& eu4regiment: eu4Army.getRegiments())
	{
		// Count the regiments by color.
		regimentCounts[eu4regiment.getCategory()]++; 
		// Store potential homes
		REGIMENTTYPE chosenType = pickCategory(eu4regiment.getCategory(), civilized);
		if (eu4regiment.getHome() > 0) eu4homeProvinces[chosenType].push_back(eu4regiment.getHome()); 
	}
	isNavy = eu4Army.getArmyFloats();

	// Create a build order and store remainders aside.
	for (auto& eu4regimentItr : regimentCounts)
	{
		int typeStrength = eu4Army.getTotalTypeStrength(eu4regimentItr.first);
		if (typeStrength == 0) continue; // no regiments of this type

		auto regimentCost = regimentCostsMapper.getCostForRegimentType(EU4::RegimentCategoryTypes[eu4regimentItr.first]);
		if (!regimentCost) continue; // Well, shit. Mapping error? 
		REGIMENTTYPE chosenType = pickCategory(eu4regimentItr.first, civilized);

		double regimentCount = static_cast<double>(typeStrength) / *regimentCost;		
		buildOrder[chosenType] = static_cast<int>(std::floor(regimentCount));
		armyRemainders[chosenType] = regimentCount - buildOrder[chosenType];
	}

	// Let's build some regiments.

	for (auto buildItem: buildOrder)
	{
		for (int regimentCounter = 0; regimentCounter < buildItem.second; ++regimentCounter)
		{
			if (addRegimentToArmy(buildItem.first, allProvinces, provinceMapper, portProvincesMapper, unitNameCount, localAdjective) != addRegimentToArmyResult::success)
			{
				// couldn't add, dissolve into pool
				armyRemainders[buildItem.first] += 1.0;
			}
			else
			{
				++buildSuceeded[buildItem.first];
			}
		}
	}
	
	// Place the army somewhere.

	auto locationCandidates = provinceMapper.getVic2ProvinceNumbers(eu4Army.getLocation());
	if (locationCandidates.empty())
	{
		Log(LogLevel::Warning) << "no mapping for province : " << eu4Army.getLocation();
		// Mapping issues, great. Dissolve the army.
		for (auto buildItem : buildSuceeded)
		{
			armyRemainders[buildItem.first] += buildItem.second;
		}
		created = false;
		return;
	}
	if (locationCandidates.size() == 1 && *locationCandidates.begin() == 0)
	{
		Log(LogLevel::Warning) << "zero mapping for province : " << eu4Army.getLocation();
		// Good news - we have mapped the location province! Bad news, it's actually unmapped. How do we even get here?
		// Dissolve the army.
		for (auto buildItem : buildSuceeded)
		{
			armyRemainders[buildItem.first] += buildItem.second;
		}
		created = false;
		return;
	}
	bool usePort = false;
	// guarantee that navies are assigned to sea provinces, or land provinces with naval bases
	if (isNavy)
	{
		auto provinceItr = allProvinces.find(*locationCandidates.begin());
		if (provinceItr != allProvinces.end()) usePort = true; // It's in land provinces, so docked.
		if (usePort)
		{
			locationCandidates = getPortProvinces(locationCandidates, allProvinces, portProvincesMapper);
			if (locationCandidates.empty())
			{
				// We have a navy and no port candidates. Yay. Better get rid of it.
				for (auto buildItem : buildSuceeded)
				{
					armyRemainders[buildItem.first] += buildItem.second;
				}
				created = false;
				return;
			}
		}
	}

	int selectedLocation = pickRandomProvinceID(locationCandidates);
	if (isNavy && usePort && !portProvincesMapper.isProvinceIDWhitelisted(selectedLocation))
	{
		LOG(LogLevel::Warning) << "Assigning navy to non-whitelisted port province " << selectedLocation << " - if the save crashes, try blacklisting this province";
	}
	location = selectedLocation;
}

V2::REGIMENTTYPE V2::Army::pickCategory(EU4::REGIMENTCATEGORY incCategory, bool civilized)
{
	REGIMENTTYPE chosenType;
	switch (incCategory)
	{
	case(EU4::REGIMENTCATEGORY::infantry):
		if (civilized)
		{
			chosenType = REGIMENTTYPE::infantry;
		}
		else
		{
			chosenType = REGIMENTTYPE::irregular;
		}
		break;
	case(EU4::REGIMENTCATEGORY::cavalry):
		chosenType = REGIMENTTYPE::cavalry;
		break;
	case(EU4::REGIMENTCATEGORY::artillery):
		chosenType = REGIMENTTYPE::artillery;
		break;
	case(EU4::REGIMENTCATEGORY::heavy_ship):
		chosenType = REGIMENTTYPE::manowar;
		break;
	case(EU4::REGIMENTCATEGORY::light_ship):
	case (EU4::REGIMENTCATEGORY::galley):
		chosenType = REGIMENTTYPE::frigate;
		break;
	case(EU4::REGIMENTCATEGORY::transport):
		chosenType = REGIMENTTYPE::clipper_transport;
		break;
	default:		
		throw std::runtime_error("Unknown regiment category: " + EU4::RegimentCategoryTypes[incCategory]);
	}
	return chosenType;
}

// return values: 0 = success, -1 = retry from pool, -2 = do not retry
V2::addRegimentToArmyResult V2::Army::addRegimentToArmy(
	REGIMENTTYPE chosenType,
	std::map<int, V2Province*> allProvinces,
	const mappers::ProvinceMapper& provinceMapper,
	const mappers::PortProvinces& portProvincesMapper,
	std::map<REGIMENTTYPE, int>& unitNameCount,
	const std::string& localAdjective)
{
	// Make a regiment.
	Regiment regiment(chosenType);

	// Every regiment needs a home to draw soldiers from or to berth. Pick a home at random.
	std::optional<int> eu4Home = getProbabilisticHomeProvince(chosenType);
	if (!eu4Home) return addRegimentToArmyResult::fail;

	// Map the home to V2 province
	auto homeCandidates = provinceMapper.getVic2ProvinceNumbers(*eu4Home);
	if (homeCandidates.empty())
	{
		// This was a province that doesn't map to anything. Let's pretend that's fine and there's
		// no problem with province mappings file.
		blockHomeProvince(*eu4Home);
		return addRegimentToArmyResult::retry;
	}

	// Old-style V2 province pointer. TO DO: Replace this with smart pointers.
	V2Province* homeProvince = nullptr;
	
	if (isNavy)
	{
		// Navies should only get homes in port provinces
		homeCandidates = getPortProvinces(homeCandidates, allProvinces, portProvincesMapper);
		if (!homeCandidates.empty()) homeProvince = pickRandomPortProvince(homeCandidates, allProvinces);
		// else: So far nothing. No berth.
	}
	else
	{
		// Armies should get a home in the candidate most capable of supporting them
		std::vector<V2Province*> sortedHomeCandidates;
		for (auto candidate : homeCandidates)
		{
			auto provinceItr = allProvinces.find(candidate);
			if (provinceItr != allProvinces.end()) sortedHomeCandidates.push_back(provinceItr->second);
		}
		sort(sortedHomeCandidates.begin(), sortedHomeCandidates.end(), provinceRegimentCapacityPredicate);
		if (!sortedHomeCandidates.empty())
		{
			for (auto sortedCandidate : sortedHomeCandidates)
			{
				homeProvince = sortedCandidate;
				// Check owner!
				if (homeProvince->getOwner() != tag) homeProvince = nullptr;
			}
		}
		if (homeProvince == nullptr)
		{
			// Well now. Either all candidates belong to someone else, or we have a mapping issue.
			// Or candidates belong to someone else because of mapping. Time for something drastic.
			homeProvince = getProvinceForExpeditionaryArmy(allProvinces, tag);
		}
		if (homeProvince == nullptr)
		{
			// Seriously now, not a single province with any soldiers left? Then what are we doing?
			return addRegimentToArmyResult::fail;
		}
		///////// We have a home! Champagne and biscuits! 		
		// Armies need to be associated with pops
		Pop* soldierPop = homeProvince->getSoldierPopForArmy();
		if (nullptr == soldierPop)
		{
			// if the old home province was colonized and can't support the unit, try turning it into an "expeditionary" army
			if (homeProvince->wasColony())
			{
				V2Province* expSender = getProvinceForExpeditionaryArmy(allProvinces, tag);
				if (expSender)
				{
					Pop* expSoldierPop = expSender->getSoldierPopForArmy();
					if (nullptr != expSoldierPop)
					{
						homeProvince = expSender;
						soldierPop = expSoldierPop;
					}
				}
			}
		}
		if (nullptr == soldierPop)
		{
			homeProvince->getSoldierPopForArmy(true);
		}
		regiment.setHome(homeProvince->getNum());	
	}
	if (homeProvince != nullptr)
	{
		regiment.setName(homeProvince->getRegimentName(chosenType));
	}
	else
	{
		regiment.setName(getRegimentName(chosenType, unitNameCount, localAdjective));
	}
	regiments.push_back(regiment);
	return addRegimentToArmyResult::success;
}

std::optional<int> V2::Army::getProbabilisticHomeProvince(REGIMENTTYPE chosenType) const
{
	if (eu4homeProvinces.find(chosenType) == eu4homeProvinces.end()) return std::nullopt;

	const auto& candidates = eu4homeProvinces.at(chosenType);
	if (candidates.empty()) return std::nullopt;

	std::set<int> randomProvince;
	std::sample(candidates.begin(), candidates.end(), std::inserter(randomProvince, randomProvince.begin()), 1, std::mt19937{ std::random_device{}() });
	return *randomProvince.begin();
}

V2Province* V2::Army::pickRandomPortProvince(std::vector<int> homeCandidates, std::map<int, V2Province*> allProvinces)
{
	std::set<int> randomProvince;
	std::sample(homeCandidates.begin(), homeCandidates.end(), std::inserter(randomProvince, randomProvince.begin()), 1, std::mt19937{ std::random_device{}() });
	int homeProvinceID = *randomProvince.begin();

	auto provinceItr = allProvinces.find(homeProvinceID);
	if (provinceItr != allProvinces.end())
	{
		return provinceItr->second;
	}
	return nullptr;
}

int V2::Army::pickRandomProvinceID(std::vector<int> homeCandidates)
{
	std::set<int> randomProvince;
	std::sample(homeCandidates.begin(), homeCandidates.end(), std::inserter(randomProvince, randomProvince.begin()), 1, std::mt19937{ std::random_device{}() });
	if (randomProvince.empty()) return 0;
	return *randomProvince.begin();
}

void V2::Army::blockHomeProvince(int blocked)
{
	for (const auto& regType : RegimentTypeToName)
	{
		auto& homes = eu4homeProvinces[regType.first];
		homes.erase(std::remove(homes.begin(), homes.end(), blocked), homes.end());
	}
}

std::vector<int> V2::Army::getPortProvinces(
	const std::vector<int>& locationCandidates,
	std::map<int, V2Province*> allProvinces,
	const mappers::PortProvinces& portProvincesMapper)
{
	std::vector<int> unblockedCandidates;
	for (auto candidate : locationCandidates)
	{
		if (!portProvincesMapper.isProvinceIDBlacklisted(candidate)) unblockedCandidates.push_back(candidate);
	}

	std::vector<int> coastalProvinces;
	for (auto& candidate : unblockedCandidates)
	{
		auto province = allProvinces.find(candidate);
		if (province != allProvinces.end())
		{
			if (province->second->isCoastal())
			{
				coastalProvinces.push_back(candidate);
			}
		}
	}
	return coastalProvinces;
}

bool V2::Army::provinceRegimentCapacityPredicate(V2Province* prov1, V2Province* prov2)
{
	return prov1->getAvailableSoldierCapacity() > prov2->getAvailableSoldierCapacity();
}

V2Province* V2::Army::getProvinceForExpeditionaryArmy(std::map<int, V2Province*> allProvinces, const std::string& tag)
{
	std::vector<V2Province*> candidates;
	for (auto& provinceItr : allProvinces)
	{
		if (provinceItr.second->getOwner() == tag && !provinceItr.second->wasColony() && !provinceItr.second->getPops("soldiers").empty())
		{
			candidates.push_back(provinceItr.second);
		}
	}
	if (!candidates.empty())
	{
		sort(candidates.begin(), candidates.end(), provinceRegimentCapacityPredicate);
		return candidates[0];
	}
	return nullptr;
}

std::string V2::Army::getRegimentName(REGIMENTTYPE chosenType, std::map<REGIMENTTYPE, int>& unitNameCount, const std::string& localAdjective)
{
	std::stringstream str;
	str << ++unitNameCount[chosenType] << CardinalToOrdinal(unitNameCount[chosenType]); // 1st, 2nd, etc
	std::string adjective = localAdjective;
	if (adjective.empty())
	{
		str << " ";
	}
	else
	{
		str << " " << adjective << " ";
	}
	switch (chosenType)
	{
	case REGIMENTTYPE::irregular:
		str << "Irregulars";
		break;
	case REGIMENTTYPE::infantry:
		str << "Infantry";
		break;
	case REGIMENTTYPE::cavalry:
		str << "Cavalry";
		break;
	case REGIMENTTYPE::artillery:
		str << "Artillery";
		break;
	case REGIMENTTYPE::manowar:
		str << "Man'o'war";
		break;
	case REGIMENTTYPE::frigate:
		str << "Frigate";
		break;
	case REGIMENTTYPE::clipper_transport:
		str << "Clipper Transport";
		break;
	}
	return str.str();
}

bool V2::Army::hasRegimentsOfType(REGIMENTTYPE chosenType) const
{
	for (const auto& regiment : regiments) if (regiment.getType() == chosenType) return true;
	return false;
}

double V2::Army::getRegimentRemainder(REGIMENTTYPE chosenType) const
{
	auto remainder = armyRemainders.find(chosenType);
	if (remainder != armyRemainders.end()) return remainder->second;
	return 0;
}
