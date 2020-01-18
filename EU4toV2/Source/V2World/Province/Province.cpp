#include "Province.h"
#include "OSCompatibilityLayer.h"
#include "../../Configuration.h"
#include "../V2Country.h"
#include <cmath>
#include "CardinalToOrdinal.h"
#include <algorithm>
#include "Log.h"

V2::Province::Province(const std::string& _filename, const mappers::ClimateMapper& climateMapper, const mappers::TerrainDataMapper& terrainDataMapper):
	filename(_filename)
{
	int slash = filename.find_last_of("/");
	unsigned int numDigits = filename.find_first_of("-") - slash - 2;
	std::string temp = filename.substr(slash + 1, numDigits);
	provinceID = stoi(temp);

	//In case we're overriding provinces (not true by default)
	if (Utils::DoesFileExist("./blankMod/output/history/provinces" + filename))
	{
		details = mappers::ProvinceDetails("./blankMod/output/history/provinces" + filename);
	}
	else
	{
		details = mappers::ProvinceDetails(theConfiguration.getVic2Path() + "/history/provinces" + filename);
	}
	for (const auto& climate : climateMapper.getClimateMap())
	{
		if (count(climate.second.begin(), climate.second.end(), provinceID))
		{
			details.climate = climate.first;
			break;
		}
	}
	if (details.terrain.empty())
	{
		auto terrain = terrainDataMapper.getTerrainForID(provinceID);
		if (terrain) details.terrain = *terrain;
	}
}

void V2::Province::addVanillaPop(std::shared_ptr<Pop> vanillaPop)
{
	vanillaPops.push_back(vanillaPop);
	vanillaPopulation += vanillaPop->getSize();
}

void V2::Province::addMinorityPop(std::shared_ptr<Pop> minorityPop)
{
	minorityPops.push_back(minorityPop);
}

void V2::Province::addCore(std::string newCore)
{
	// only add if not a territorial core/colony of the current owner
	if (!(newCore == details.owner && territorialCore))
	{
		details.cores.insert(newCore);
	}
}

void V2::Province::convertFromOldProvince(
	const EU4::Province* oldProvince,
	const std::map<std::string, std::shared_ptr<EU4::Country>>& theEU4Countries
) {
	//srcProvince = oldProvince;
	inHRE = oldProvince->inHre();
	if (!oldProvince->isCity())
	{
		colonial = 1;
		territorialCore = true;
	}
	else if (oldProvince->isTerritorialCore())
	{
		colonial = 2;
		territorialCore = true;
	}
	else
	{
		colonial = 0;
	}
	wasColonised = oldProvince->wasColonised();
	
	if (oldProvince->hasBuilding("weapons")) ++mfgCount;
	if (oldProvince->hasBuilding("wharf")) ++mfgCount;
	if (oldProvince->hasBuilding("textile")) ++mfgCount;
	if (oldProvince->hasBuilding("plantations")) ++mfgCount;
	if (oldProvince->hasBuilding("tradecompany")) ++mfgCount;
	if (oldProvince->hasBuilding("farm_estate")) ++mfgCount;
	if (oldProvince->hasBuilding("mills")) ++mfgCount;
	if (oldProvince->hasBuilding("furnace")) mfgCount += 3;
	if (oldProvince->hasBuilding("shipyard")) ++navalBaseLevel;
	if (oldProvince->hasBuilding("grand_shipyard")) navalBaseLevel += 2;
	if (oldProvince->hasBuilding("naval_arsenal")) ++navalBaseLevel;
	if (oldProvince->hasBuilding("naval_base")) navalBaseLevel += 2;

	auto countryItr = theEU4Countries.find(oldProvince->getOwnerString());
	if (countryItr != theEU4Countries.end())
	{
		importedIdeas = countryItr->second->exportNationalIdeas();
	}
	importedBuildings = oldProvince->exportBuildings();

	devpushMod = oldProvince->getDevDelta() / 100.0;
	weightMod = oldProvince->getModifierWeight() / 100.0;
	totalWeight = oldProvince->getTotalWeight();
}

std::optional<std::shared_ptr<V2::Factory>> V2::Province::addFactory(std::shared_ptr<Factory> factory)
{
	auto itr = factories.find(factory->getTypeName());
	if (itr == factories.end())
	{
		factories.insert(std::make_pair(factory->getTypeName(), factory));
		return factory;
	}
	itr->second->increaseLevel();
	return std::nullopt;
}

void V2::Province::addPopDemographic(const Demographic& d)
{
	bool combined = false;
	for (auto& demographic : demographics)
	{
		if ((demographic.culture == d.culture) && (demographic.religion == d.religion))
		{
			combined = true;
			demographic.upperRatio += d.upperRatio;
			demographic.middleRatio += d.middleRatio;
			demographic.lowerRatio += d.lowerRatio;
		}
	}
	if (!combined)
	{
		demographics.push_back(d);
	}
}

void V2::Province::determineColonial()
{
	if (territorialCore && colonial == 0) colonial = 2;
}

int V2::Province::getTotalPopulation() const
{
	int total = 0;
	for (const auto& pop: pops) total += pop->getSize();
	return total;
}

std::vector<std::string> V2::Province::getCulturesOverThreshold(double percentOfPopulation) const
{
	int totalPopulation = getTotalPopulation();
	if (!totalPopulation) return std::vector<std::string>();

	std::map<std::string, int> cultureTotals;
	for (auto pop : pops) cultureTotals[pop->getCulture()] += pop->getSize();

	std::vector<std::string> culturesOverThreshold;
	for (auto cultureAmount : cultureTotals)
	{
		if (static_cast<double>(cultureAmount.second)/totalPopulation >= percentOfPopulation)
		{
			culturesOverThreshold.push_back(cultureAmount.first);
		}
	}

	return culturesOverThreshold;
}

std::optional<std::pair<int, std::vector<std::shared_ptr<V2::Pop>>>> V2::Province::getPopsForOutput()
{
	if (resettable && theConfiguration.getResetProvinces() == "yes" && !vanillaPops.empty())
	{
		return std::pair(provinceID, vanillaPops);
	}
	if (!pops.empty()) return std::pair(provinceID, pops);
	if (!vanillaPops.empty()) return std::pair(provinceID, vanillaPops);
	return std::nullopt;
}

void V2::Province::doCreatePops(
	double popWeightRatio,
	V2Country* _owner,
	int popConversionAlgorithm,
	const mappers::ProvinceMapper& provinceMapper
) {
	// convert pops
	for (auto demographic: demographics)
	{
		createPops(demographic, popWeightRatio, _owner, popConversionAlgorithm, provinceMapper);
	}
	combinePops();

	// organize pops for adding minorities
	std::map<std::string, int> totals;
	std::map<std::string, std::vector<std::shared_ptr<Pop>>> thePops;
	for (auto popItr : pops)
	{
		std::string type = popItr->getType();

		auto totalsItr = totals.find(type);
		if (totalsItr == totals.end())
		{
			totals.insert(std::make_pair(type, popItr->getSize()));
		}
		else
		{
			totalsItr->second += popItr->getSize();
		}

		auto thePopsItr = thePops.find(type);
		if (thePopsItr == thePops.end())
		{
			std::vector<std::shared_ptr<Pop>> newVector;
			newVector.push_back(popItr);
			thePops.insert(std::make_pair(type, newVector));
		}
		else
		{
			thePopsItr->second.push_back(popItr);
		}
	}

	// decrease non-minority pops and create the minorities
	std::vector<std::shared_ptr<Pop>> actualMinorities;
	for (auto minorityItr : minorityPops)
	{
		int totalTypePopulation;
		auto totalsItr = totals.find(minorityItr->getType());
		if (totalsItr != totals.end())
		{
			totalTypePopulation = totalsItr->second;
		}
		else
		{
			totalTypePopulation = 0;
		}

		auto thePopsItr = thePops.find(minorityItr->getType());
		if (thePopsItr != thePops.end())
		{
			for (auto popsItr : thePopsItr->second)
			{
				std::string newCulture = minorityItr->getCulture();
				std::string newReligion = minorityItr->getReligion();
				if (newCulture.empty()) newCulture = popsItr->getCulture();
				if (newReligion.empty()) newReligion = popsItr->getReligion();

				auto newMinority = std::make_shared<Pop>(minorityItr->getType(), lround(popsItr->getSize() / totalTypePopulation * minorityItr->getSize()), newCulture, newReligion);
				actualMinorities.push_back(newMinority);

				popsItr->changeSize(static_cast<int>(-1.0 * popsItr->getSize() / totalTypePopulation * minorityItr->getSize()));
			}
		}
	}

	// add minority pops to the main pops
	for (auto minorityItr : actualMinorities)
	{
		pops.push_back(minorityItr);
	}
	combinePops();
}


V2::Province::pop_points V2::Province::getPopPoints_1(
	const Demographic& demographic,
	double newPopulation,
	const V2Country* _owner
) const  {
	pop_points pts;

	int govBuilding = 0;
	if (importedBuildings.count("temple")) { govBuilding = 1; } 
	else if (importedBuildings.count("courthouse")) { govBuilding = 2; }
	else if (importedBuildings.count("spy_agency")) { govBuilding = 3; }
	else if (importedBuildings.count("town_hall")) { govBuilding = 4; }
	else if (importedBuildings.count("college")) { govBuilding = 6; }
	else if (importedBuildings.count("cathedral")) { govBuilding = 8; }

	int armyBuilding = 0;
	if (importedBuildings.count("armory")) { armyBuilding = 1; }
	else if (importedBuildings.count("training_fields")) { armyBuilding = 2; }
	else if (importedBuildings.count("barracks")) { armyBuilding = 3; }
	else if (importedBuildings.count("regimental_camp")) { armyBuilding = 4; }
	else if (importedBuildings.count("arsenal")) { armyBuilding = 6; }
	else if (importedBuildings.count("conscription_center")) { armyBuilding = 8; }

	int productionBuilding = 0;
	if (importedBuildings.count("constable")) { productionBuilding = 1; }
	else if (importedBuildings.count("workshop")) { productionBuilding = 2; }
	else if (importedBuildings.count("counting_house")) { productionBuilding = 3; }
	else if (importedBuildings.count("treasury_office")) { productionBuilding = 4; }
	else if (importedBuildings.count("mint")) { productionBuilding = 6; }
	else if (importedBuildings.count("stock_exchange")) { productionBuilding = 8; }

	int tradeBuilding = 0;
	if (importedBuildings.count("marketplace")) { tradeBuilding = 1; }
	else if (importedBuildings.count("trade_depot")) { tradeBuilding = 2; }
	else if (importedBuildings.count("canal")) { tradeBuilding = 3; }
	else if (importedBuildings.count("road_network")) { tradeBuilding = 4; }
	else if (importedBuildings.count("post_office")) { tradeBuilding = 6; }
	else if (importedBuildings.count("customs_house")) { tradeBuilding = 8; }

	pts.artisans += 400;
	pts.artisans += static_cast<double>(productionBuilding) * 125;

	pts.soldiers += 100;
	pts.soldiers += static_cast<double>(armyBuilding) * 45;
	if (importedIdeas.count("quantity_ideas")) pts.soldiers *= 2;

	pts.officers += 2 * (static_cast<double>(armyBuilding) + 2);
	if (importedIdeas.count("quality_ideas")) pts.officers += 5;

	pts.clergymen += 95;
	if (importedIdeas.count("religious_ideas")) pts.clergymen += 10;
	if (importedIdeas.count("innovative_ideas")) pts.clergymen += 10;

	pts.bureaucrats += 10;
	pts.bureaucrats += static_cast<double>(govBuilding) * 2;
	if (importedIdeas.count("administrative_ideas")) pts.bureaucrats += 10;
	if (importedIdeas.count("expansion_ideas") && wasColonised) pts.bureaucrats += 10;

	pts.aristocrats += 7 * (static_cast<double>(tradeBuilding) + 11);
	if (importedBuildings.count("farm_estate") || importedBuildings.count("plantations")) pts.aristocrats *= 2;
	if (importedIdeas.count("aristocracy_ideas")) pts.aristocrats *= 2;

	if (!factories.empty())
	{
		double capsPerFactory = 40 + static_cast<double>(_owner->getNumFactories()) * 2;
		double actualCapitalists = static_cast<double>(factories.size())* static_cast<double>(_owner->getNumFactories())* capsPerFactory* demographic.upperRatio;
		pts.capitalists += (10000 * actualCapitalists) / (demographic.upperRatio * newPopulation);

		double actualClerks = 181 * static_cast<double>(factories.size())* demographic.middleRatio;
		pts.clerks += (10000 * actualClerks) / (demographic.middleRatio * newPopulation);

		double actualCraftsmen = 2639 * static_cast<double>(factories.size())* demographic.lowerRatio;
		pts.craftsmen += (10000 * actualCraftsmen) / (demographic.lowerRatio * newPopulation);
	}

	return pts;
}

V2::Province::pop_points V2::Province::getPopPoints_2(
	const Demographic& demographic,
	double newPopulation,
	const V2Country* _owner
) const {
	pop_points pts;

	int adminBuilding = 0;
	if (importedBuildings.count("courthouse")) { adminBuilding = 1; }
	else if (importedBuildings.count("town_hall")) { adminBuilding = 2; }

	int taxBuilding = 0;
	if (importedBuildings.count("temple")) { taxBuilding = 1; }
	else if (importedBuildings.count("cathedral")) { taxBuilding = 2; }

	int manpowerBuilding = 0;
	if (importedBuildings.count("barracks")) { manpowerBuilding = 1; }
	else if (importedBuildings.count("training_fields")) { manpowerBuilding = 2; }

	int armyBuilding = 0;
	if (importedBuildings.count("regimental_camp")) { armyBuilding = 1; }
	else if (importedBuildings.count("conscription_center")) { armyBuilding = 2; }

	int productionBuilding = 0;
	if (importedBuildings.count("workshop")) { productionBuilding = 1; }
	else if (importedBuildings.count("counting_house")) { productionBuilding = 2; }

	int tradeBuilding = 0;
	if (importedBuildings.count("marketplace")) { tradeBuilding = 1; }
	else if (importedBuildings.count("trade_depot")) { tradeBuilding = 2; }
	else if (importedBuildings.count("stock_exchange")) { tradeBuilding = 3; }

	pts.artisans += 400;
	pts.artisans += static_cast<double>(productionBuilding) * 500;

	pts.soldiers += 100;
	pts.soldiers += (static_cast<double>(manpowerBuilding) + static_cast<double>(armyBuilding)) * 90;
	if (importedIdeas.count("quantity_ideas")) pts.soldiers *= 2;

	pts.officers += 4 * (static_cast<double>(manpowerBuilding) + static_cast<double>(armyBuilding) + 2.0);
	if (importedIdeas.count("quality_ideas")) pts.officers += 5;

	pts.clergymen += 65;
	if (importedIdeas.count("religious_ideas")) pts.clergymen += 10;
	if (importedIdeas.count("innovative_ideas")) pts.clergymen += 10;
	if (importedBuildings.count("university")) pts.clergymen *= 2;

	pts.bureaucrats += 10;
	pts.bureaucrats += (static_cast<double>(adminBuilding) + static_cast<double>(taxBuilding)) * 4;
	if (importedIdeas.count("administrative_ideas")) pts.bureaucrats += 10;
	if (importedIdeas.count("expansion_ideas") && wasColonised) pts.bureaucrats += 10;

	pts.aristocrats += 14 * (static_cast<double>(tradeBuilding) + 6.0);
	if (importedBuildings.count("farm_estate") || importedBuildings.count("plantations")) pts.aristocrats *= 2;
	if (importedIdeas.count("aristocracy_ideas")) pts.aristocrats *= 2;

	if (!factories.empty())
	{
		double capsPerFactory = 40 + static_cast<double>(_owner->getNumFactories()) * 2;
		double actualCapitalists = static_cast<double>(factories.size())* static_cast<double>(_owner->getNumFactories())* capsPerFactory* demographic.upperRatio;
		pts.capitalists += (10000 * actualCapitalists) / (demographic.upperRatio * newPopulation);

		double actualClerks = 181 * static_cast<double>(factories.size())* demographic.middleRatio;
		pts.clerks += (10000 * actualClerks) / (demographic.middleRatio * newPopulation);

		double actualCraftsmen = 2639 * static_cast<double>(factories.size())* demographic.lowerRatio;
		pts.craftsmen += (10000 * actualCraftsmen) / (demographic.lowerRatio * newPopulation);
	}

	return pts;
}

void V2::Province::createPops(
	const Demographic& demographic,
	double popWeightRatio,
	const V2Country* _owner,
	int popConversionAlgorithm,
	const mappers::ProvinceMapper& provinceMapper
) {
	long newPopulation = 0;
	double lifeRatingMod = (static_cast<double>(details.lifeRating) - 30.0) / 200.0;
	double shapeMod = theConfiguration.getPopShapingFactor() / 100.0;
	double provinceDevModifier = 1 + (lifeRatingMod + devpushMod + weightMod) * shapeMod;

	switch (theConfiguration.getPopShaping()) {
	case Configuration::POPSHAPES::Vanilla:
		newPopulation = vanillaPopulation;
		break;

	case Configuration::POPSHAPES::PopShaping:
		newPopulation = static_cast<long>(vanillaPopulation * provinceDevModifier);
		break;

	case Configuration::POPSHAPES::Extreme:
		newPopulation = static_cast<long>(static_cast<double>(details.lifeRating) / 10 * popWeightRatio * totalWeight);

		auto Vic2Provinces = provinceMapper.getVic2ProvinceNumbers(eu4ID);
		int numOfV2Provs = Vic2Provinces.size();
		if (numOfV2Provs > 1)
		{
			if (numOfV2Provs == 2)
			{
				newPopulation /= numOfV2Provs;
				newPopulation = static_cast<long>(newPopulation * 1.10);
			}
			else
			{
				newPopulation /= numOfV2Provs;
				newPopulation = static_cast<long>(newPopulation * 1.15);
			}
		}

		newPopulation = vanillaPopulation + static_cast<long>((newPopulation - vanillaPopulation) * (theConfiguration.getPopShapingFactor() / 100.0));
		break;
	}

	pop_points pts;
	switch (popConversionAlgorithm)
	{
	case 1:
		pts = getPopPoints_1(demographic, newPopulation, _owner);
		break;
	case 2:
		pts = getPopPoints_2(demographic, newPopulation, _owner);
		break;
	default:
		LOG(LogLevel::Error) << "Invalid pop conversion algorithm specified; not generating pops.";
	}

	// Uncivs cannot have capitalists, clerks, or craftsmen, and get fewer bureaucrats
	if (!_owner->isCivilized())
	{
		pts.capitalists = 0;
		pts.clerks = 0;
		pts.craftsmen = 0;
		pts.bureaucrats -= 5;
	}

	int farmers = lround(demographic.lowerRatio * newPopulation);
	if (slaveProportion > 0.0)
	{
		int size = lround(demographic.lowerRatio * newPopulation * slaveProportion);
		farmers -= size;
		auto slavesPop = std::make_shared<Pop>("slaves", size, demographic.slaveCulture, demographic.religion);
		pops.push_back(slavesPop);
	}
	if (pts.soldiers > 0)
	{
		int size = lround(demographic.lowerRatio * newPopulation * (pts.soldiers / 10000));
		farmers -= size;
		auto soldiersPop = std::make_shared<Pop>("soldiers", size, demographic.culture, demographic.religion);
		pops.push_back(soldiersPop);
	}
	if (pts.craftsmen > 0)
	{
		int size = lround(demographic.lowerRatio * newPopulation * (pts.craftsmen / 10000));
		farmers -= size;
		auto craftsmenPop = std::make_shared<Pop>("craftsmen", size, demographic.culture, demographic.religion);
		pops.push_back(craftsmenPop);
	}
	if (pts.artisans > 0)
	{
		int size = lround(demographic.middleRatio * newPopulation * (pts.artisans / 10000));
		farmers -= size;
		auto artisansPop = std::make_shared<Pop>("artisans", size, demographic.culture, demographic.religion);
		pops.push_back(artisansPop);
	}
	if (pts.clergymen > 0)
	{
		int size = lround(demographic.middleRatio * newPopulation * (pts.clergymen / 10000));
		farmers -= size;
		auto clergymenPop = std::make_shared<Pop>("clergymen", size, demographic.culture, demographic.religion);
		pops.push_back(clergymenPop);
	}
	if (pts.clerks > 0)
	{
		int size = lround(demographic.middleRatio * newPopulation * (pts.clerks / 10000));
		farmers -= size;
		auto clerksPop = std::make_shared<Pop>("clerks", size, demographic.culture, demographic.religion);
		pops.push_back(clerksPop);
	}
	if (pts.bureaucrats > 0)
	{
		int size = lround(demographic.middleRatio * newPopulation * (pts.bureaucrats / 10000));
		farmers -= size;
		auto bureaucratsPop = std::make_shared<Pop>("bureaucrats", size, demographic.culture, demographic.religion);
		pops.push_back(bureaucratsPop);
	}
	if (pts.officers > 0)
	{
		int size = lround(demographic.middleRatio * newPopulation * (pts.officers / 10000));
		farmers -= size;
		auto officersPop = std::make_shared<Pop>("officers", size, demographic.culture, demographic.religion);
		pops.push_back(officersPop);
	}
	if (pts.capitalists > 0)
	{
		int size = lround(demographic.upperRatio * newPopulation * (pts.capitalists / 10000));
		farmers -= size;
		auto capitalistsPop = std::make_shared<Pop>("capitalists", size, demographic.culture, demographic.religion);
		pops.push_back(capitalistsPop);
	}
	if (pts.aristocrats > 0)
	{
		int size = lround(demographic.upperRatio * newPopulation * (pts.aristocrats / 10000));
		farmers -= size;
		auto aristocratsPop = std::make_shared<Pop>("aristocrats", size, demographic.culture, demographic.religion);
		pops.push_back(aristocratsPop);
	}

	auto farmersPop = std::make_shared<Pop>("farmers", farmers, demographic.culture, demographic.religion);
	pops.push_back(farmersPop);
}

void V2::Province::combinePops()
{
	std::vector<std::shared_ptr<Pop>> trashPops;
	for (auto lhs = pops.begin(); lhs != pops.end(); ++lhs)
	{
		auto rhs = lhs;
		for (++rhs; rhs != pops.end(); ++rhs)
		{
			if ((*lhs)->combine(*rhs))
			{
				trashPops.push_back(*rhs);
			}
			if ((*rhs)->getSize() < 1)
			{
				trashPops.push_back(*rhs);
			}
		}
	}

	std::vector<std::shared_ptr<Pop>> consolidatedPops;
	for (auto itr: pops)
	{
		bool isTrashed = false;
		for (auto titr : trashPops)
		{
			if (itr == titr) isTrashed = true;
		}
		if (!isTrashed) consolidatedPops.push_back(itr);
	}
	pops.swap(consolidatedPops);
}


// pick a soldier pop to use for an army.  prefer larger pops to smaller ones, and grow only if necessary.
std::shared_ptr<V2::Pop> V2::Province::getSoldierPopForArmy(bool force)
{
	std::vector<std::shared_ptr<Pop>> soldierPops = getPops("soldiers");
	if (soldierPops.empty()) return nullptr; // no soldier pops

	std::sort(soldierPops.begin(), soldierPops.end(), popSortBySizePredicate);
	// try largest to smallest, without growing
	for (auto soldier : soldierPops)
	{
		int growBy = getRequiredPopForRegimentCount(soldier->getSupportedRegimentCount() + 1) - soldier->getSize();
		if (growBy <= 0)
		{
			if (growSoldierPop(soldier)) // won't actually grow, but will increment supported regiment count
			{
				return soldier;
			}
		}
	}
	// try largest to smallest, trying to grow
	for (auto soldier: soldierPops)
	{
		if (growSoldierPop(soldier)) // Will actually grow and increment supported regiment count
		{
			return soldier;
		}
	}

	// no suitable pops
	if (force) return soldierPops[0];
	return nullptr;
}

std::vector<std::shared_ptr<V2::Pop>> V2::Province::getPops(const std::string& type) const
{
	std::vector<std::shared_ptr<Pop>> retval;
	for (auto itr : pops)
	{
		if (type == "*" || itr->getType() == type)
			retval.push_back(itr);
	}
	return retval;
}

bool V2::Province::popSortBySizePredicate(std::shared_ptr<Pop> pop1, std::shared_ptr<Pop> pop2)
{
	return pop1->getSize() > pop2->getSize();
}

// V2 requires 1000 for the first regiment and 3000 thereafter
// we require an extra 1/30 to stabilize the start of the game
int V2::Province::getRequiredPopForRegimentCount(int count)
{
	if (count == 0) return 0;
	return 1033 + (count - 1) * 3100;
}

bool V2::Province::growSoldierPop(std::shared_ptr<Pop> pop)
{
	int growBy = getRequiredPopForRegimentCount(pop->getSupportedRegimentCount() + 1) - pop->getSize();
	if (growBy > 0)
	{
		// gotta grow; find a same-culture same-religion farmer/laborer to pull from
		int provincePop = getTotalPopulation();
		bool foundSourcePop = false;
		for (auto popSource: pops)
		{
			if (popSource->getType() == "farmers" || popSource->getType() == "labourers")
			{
				if (popSource->getCulture() == pop->getCulture() && popSource->getReligion() == pop->getReligion())
				{
					// don't let the farmer/labourer shrink beneath 10% of the province population
					if (static_cast<double>(popSource->getSize()) - growBy > provincePop * 0.10)
					{
						popSource->changeSize(-growBy);
						pop->changeSize(growBy);
						foundSourcePop = true;
						break;
					}
				}
			}
		}
		if (!foundSourcePop) return false;
	}
	pop->incrementSupportedRegimentCount();
	return true;
}

std::string V2::Province::getRegimentName(REGIMENTTYPE chosenType)
{
	std::stringstream str;
	str << ++unitNameCount[chosenType] << CardinalToOrdinal(unitNameCount[chosenType]); // 1st, 2nd, etc
	str << " " << name << " "; // Hamburg, Lyon, etc
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

std::pair<int, int> V2::Province::getAvailableSoldierCapacity() const
{
	int soldierCap = 0;
	int draftCap = 0;
	int provincePop = getTotalPopulation();
	for (auto soldier: pops)
	{
		if (soldier->getType() == "soldiers")
		{
			// unused capacity is the size of the pop minus the capacity already used, or 0, if it's already overdrawn
			soldierCap += std::max(soldier->getSize() - getRequiredPopForRegimentCount(soldier->getSupportedRegimentCount()), 0);
		}
		else if (soldier->getType() == "farmers" || soldier->getType() == "labourers")
		{
			// unused capacity is the size of the pop in excess of 10% of the province pop, or 0, if it's already too small
			draftCap += std::max(soldier->getSize() - int(0.10 * provincePop), 0);
		}
	}
	return std::pair<int, int>(soldierCap, draftCap);
}

/*

/////// this commented code deals with exporting ship names imported from eu4 culture files. However, we don't import those
// but generate our own names, so.

// determined experimentally
static const int unitNameOffsets[static_cast<int>(EU4::REGIMENTCATEGORY::num_reg_categories)] =
{
	16,	// infantry
	5,	// cavalry
	4,	// artillery
	19, // heavy_ship
	13, // light_ship
	0,	// galley (unused)
	6	// transport
};


void V2Province::outputUnits(FILE* output) const
{
	// unit name counts are stored in an odd kind of variable-length sparse array.  try to emulate.
	int outputUnitNameUntil = 0;
	for (int i = 0; i < static_cast<int>(EU4::REGIMENTCATEGORY::num_reg_categories); ++i)
	{
		if (unitNameCount[i] > 0 && unitNameOffsets[i] > outputUnitNameUntil)
		{
			outputUnitNameUntil = unitNameOffsets[i];
		}
	}
	if (outputUnitNameUntil > 0)
	{
		fprintf(output, "\tunit_names=\n");
		fprintf(output, "\t{\n");
		fprintf(output, "\t\tdata=\n");
		fprintf(output, "\t\t{\n");
		for (int i = 1; i <= outputUnitNameUntil; ++i)
		{
			fprintf(output, "\t\t\t{\n");
			for (int j = 0; j < static_cast<int>(EU4::REGIMENTCATEGORY::num_reg_categories); ++j)
			{
				if ((i == unitNameOffsets[j]) && unitNameCount[j] > 0)
				{
					fprintf(output, "\t\t\t\tcount=%d\n", unitNameCount[j]);
				}
			}
			fprintf(output, "\t\t\t}\n\n");
		}
		fprintf(output, "\t\t}\n");
		fprintf(output, "\t}\n");
	}
}

bool V2Province::hasCulture(const std::string& culture, float percentOfPopulation) const
{
	int culturePops = 0;
	for (std::vector<V2::Pop*>::const_iterator itr = pops.begin(); itr != pops.end(); ++itr)
	{
		if ((*itr)->getCulture() == culture)
		{
			culturePops += (*itr)->getSize();
		}
	}

	return ((float)culturePops / getTotalPopulation()) >= percentOfPopulation;
}
*/