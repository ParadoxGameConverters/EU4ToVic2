#include "Province.h"
#include "OSCompatibilityLayer.h"
#include "../../Configuration.h"

/*
#include "CardinalToOrdinal.h"
#include "Log.h"
#include "../EU4World/World.h"
#include "../EU4World/Provinces/EU4Province.h"
#include "../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "Pop/Pop.h"
#include "V2Country.h"
#include <algorithm>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdio.h>
#include "ParserHelpers.h"
*/

V2::Province::Province(const std::string& _filename, const mappers::ClimateMapper& climateMapper, const mappers::TerrainDataMapper& terrainDataMapper):
	filename(_filename)
{
	int slash = filename.find_last_of("/");
	int numDigits = filename.find_first_of("-") - slash - 2;
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
	// TODO: seems suspicious, check the condition and use cases
	if (!(newCore == details.owner) && territorialCore)
	{
		details.cores.insert(newCore);
	}
}

void V2::Province::convertFromOldProvince(
	const EU4::Religions& allReligions,
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
}

void V2::Province::addFactory(const Factory& factory)
{
	auto itr = factories.find(factory.getTypeName());
	if (itr == factories.end())
	{
		factories.insert(std::make_pair(factory.getTypeName(), factory));
	}
	else
	{
		itr->second.increaseLevel();
	}
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
	if (territorialCore && (colonial == 0))
	{
		colonial = 2;
	}
}

int V2::Province::getTotalPopulation() const
{
	int total = 0;
	for (const auto& pop: pops)
	{
		total += pop->getSize();
	}
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

/*

void V2Province::outputPops(std::ofstream& output) const
{
		output << num << " = {\n";
		if (oldPops.size() > 0)
		{
			for (unsigned int i = 0; i < oldPops.size(); i++)
			{
				output << *oldPops[i];
			}
			output << "}\n";
		}
	}
	else
	{
		if (pops.size() > 0)
		{
			output << num << " = {\n";
			for (auto i : pops)
			{
				output << *i;
			}
			output << "}\n";
		}
		else if (oldPops.size() > 0)
		{
			output << num << " = {\n";
			for (unsigned int i = 0; i < oldPops.size(); i++)
			{
				output << *oldPops[i];
			}
			output << "}\n";
		}
	}
}


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












void V2Province::doCreatePops(
	double popWeightRatio,
	V2Country* _owner,
	int popConversionAlgorithm,
	const std::map<std::string, std::shared_ptr<EU4::Country>>& theEU4Countries,
	const mappers::ProvinceMapper& provinceMapper
) {
	// convert pops
	for (std::vector<V2Demographic>::const_iterator itr = demographics.begin(); itr != demographics.end(); ++itr)
	{
		createPops(*itr, popWeightRatio, _owner, popConversionAlgorithm, theEU4Countries, provinceMapper);
	}
	combinePops();

	// organize pops for adding minorities
	std::map<std::string, int>					totals;
	std::map<std::string, std::vector<V2::Pop*>>	thePops;
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
			std::vector<V2::Pop*> newVector;
			newVector.push_back(popItr);
			thePops.insert(std::make_pair(type, newVector));
		}
		else
		{
			thePopsItr->second.push_back(popItr);
		}
	}

	// decrease non-minority pops and create the minorities
	std::vector<V2::Pop*> actualMinorities;
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
				if (newCulture == "")
				{
					newCulture = popsItr->getCulture();
				}
				if (newReligion == "")
				{
					newReligion = popsItr->getReligion();
				}

				V2::Pop* newMinority = new V2::Pop(minorityItr->getType(), static_cast<int>(1.0 * popsItr->getSize() / totalTypePopulation * minorityItr->getSize() + 0.5), newCulture, newReligion);
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


// each "point" here represents 0.01% (1 / 10 000) population of this culture-religion pair
struct V2Province::pop_points
{
	double craftsmen = 0;
	double slaves = 0;
	double soldiers = 0;
	double artisans = 0;
	double clergymen = 0;
	double clerks = 0;
	double bureaucrats = 0;
	double officers = 0;
	double capitalists = 0;
	double aristocrats = 0;
};


V2Province::pop_points V2Province::getPopPoints_1(
	const V2Demographic& demographic,
	double newPopulation,
	const V2Country* _owner,
	const std::map<std::string, std::shared_ptr<EU4::Country>>& theEU4Countries
) {
	const EU4::Province*	oldProvince = demographic.oldProvince;
	auto oldCountryTag = demographic.oldCountry;
	auto countryItr = theEU4Countries.find(oldCountryTag);
	std::shared_ptr<EU4::Country> oldCountry;
	if (countryItr != theEU4Countries.end())
	{
		oldCountry = countryItr->second;
	}

	pop_points pts;

	int govBuilding = 0;
	if (oldProvince->hasBuilding("temple"))
	{
		govBuilding = 1;
	}
	else if (oldProvince->hasBuilding("courthouse"))
	{
		govBuilding = 2;
	}
	else if (oldProvince->hasBuilding("spy_agency"))
	{
		govBuilding = 3;
	}
	else if (oldProvince->hasBuilding("town_hall"))
	{
		govBuilding = 4;
	}
	else if (oldProvince->hasBuilding("college"))
	{
		govBuilding = 6;
	}
	else if (oldProvince->hasBuilding("cathedral"))
	{
		govBuilding = 8;
	}

	int armyBuilding = 0;
	if (oldProvince->hasBuilding("armory"))
	{
		armyBuilding = 1;
	}
	else if (oldProvince->hasBuilding("training_fields"))
	{
		armyBuilding = 2;
	}
	else if (oldProvince->hasBuilding("barracks"))
	{
		armyBuilding = 3;
	}
	else if (oldProvince->hasBuilding("regimental_camp"))
	{
		armyBuilding = 4;
	}
	else if (oldProvince->hasBuilding("arsenal"))
	{
		armyBuilding = 6;
	}
	else if (oldProvince->hasBuilding("conscription_center"))
	{
		armyBuilding = 8;
	}

	int productionBuilding = 0;
	if (oldProvince->hasBuilding("constable"))
	{
		productionBuilding = 1;
	}
	else if (oldProvince->hasBuilding("workshop"))
	{
		productionBuilding = 2;
	}
	else if (oldProvince->hasBuilding("counting_house"))
	{
		productionBuilding = 3;
	}
	else if (oldProvince->hasBuilding("treasury_office"))
	{
		productionBuilding = 4;
	}
	else if (oldProvince->hasBuilding("mint"))
	{
		productionBuilding = 6;
	}
	else if (oldProvince->hasBuilding("stock_exchange"))
	{
		productionBuilding = 8;
	}

	int tradeBuilding = 0;
	if (oldProvince->hasBuilding("marketplace"))
	{
		tradeBuilding = 1;
	}
	else if (oldProvince->hasBuilding("trade_depot"))
	{
		tradeBuilding = 2;
	}
	else if (oldProvince->hasBuilding("canal"))
	{
		tradeBuilding = 3;
	}
	else if (oldProvince->hasBuilding("road_network"))
	{
		tradeBuilding = 4;
	}
	else if (oldProvince->hasBuilding("post_office"))
	{
		tradeBuilding = 6;
	}
	else if (oldProvince->hasBuilding("customs_house"))
	{
		tradeBuilding = 8;
	}

	pts.artisans += 400;
	pts.artisans += static_cast<double>(productionBuilding) * 125;

	pts.soldiers += 100;
	pts.soldiers += static_cast<double>(armyBuilding) * 45;
	if ((oldCountryTag != "") && (oldCountry->hasNationalIdea("quantity_ideas")))
	{
		pts.soldiers *= 2;
	}

	pts.officers += 2 * (static_cast<double>(armyBuilding) + 2);
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("quality_ideas")))
	{
		pts.officers += 5;
	}

	pts.clergymen += 95;
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("religious_ideas")))
	{
		pts.clergymen += 10;
	}
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("innovative_ideas")))
	{
		pts.clergymen += 10;
	}

	pts.bureaucrats += 10;
	pts.bureaucrats += static_cast<double>(govBuilding) * 2;
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("administrative_ideas")))
	{
		pts.bureaucrats += 10;
	}
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("expansion_ideas")) && oldProvince->wasColonised())
	{
		pts.bureaucrats += 10;
	}

	pts.aristocrats += 7 * (static_cast<double>(tradeBuilding) + 11);
	if (oldProvince->hasBuilding("farm_estate") ||
		oldProvince->hasBuilding("plantations")
		)
	{
		pts.aristocrats *= 2;
	}
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("aristocracy_ideas")))
	{
		pts.aristocrats *= 2;
	}

	if (factories.size() > 0)
	{
		double capsPerFactory = 40 + static_cast<double>(_owner->getNumFactories()) * 2;
		double actualCapitalists = static_cast<double>(factories.size()) * static_cast<double>(_owner->getNumFactories()) * capsPerFactory * demographic.upperRatio;
		pts.capitalists += (10000 * actualCapitalists) / (demographic.upperRatio * newPopulation);

		double actualClerks = 181 * static_cast<double>(factories.size()) * demographic.middleRatio;
		pts.clerks += (10000 * actualClerks) / (demographic.middleRatio * newPopulation);

		double actualCraftsmen = 2639 * static_cast<double>(factories.size()) * demographic.lowerRatio;
		pts.craftsmen += (10000 * actualCraftsmen) / (demographic.lowerRatio * newPopulation);
	}

	return pts;
}


V2Province::pop_points V2Province::getPopPoints_2(
	const V2Demographic& demographic,
	double newPopulation,
	const V2Country* _owner,
	const std::map<std::string, std::shared_ptr<EU4::Country>>& theEU4Countries
) {
	const EU4::Province*	oldProvince = demographic.oldProvince;
	auto oldCountryTag = demographic.oldCountry;
	auto countryItr = theEU4Countries.find(oldCountryTag);
	std::shared_ptr<EU4::Country> oldCountry;
	if (countryItr != theEU4Countries.end())
	{
		oldCountry = countryItr->second;
	}

	pop_points pts;

	int adminBuilding = 0;
	if (oldProvince->hasBuilding("courthouse"))
	{
		adminBuilding = 1;
	}
	else if (oldProvince->hasBuilding("town_hall"))
	{
		adminBuilding = 2;
	}

	int taxBuilding = 0;
	if (oldProvince->hasBuilding("temple"))
	{
		taxBuilding = 1;
	}
	else if (oldProvince->hasBuilding("cathedral"))
	{
		taxBuilding = 2;
	}

	int manpowerBuilding = 0;
	if (oldProvince->hasBuilding("barracks"))
	{
		manpowerBuilding = 1;
	}
	else if (oldProvince->hasBuilding("training_fields"))
	{
		manpowerBuilding = 2;
	}

	int armyBuilding = 0;
	if (oldProvince->hasBuilding("regimental_camp"))
	{
		armyBuilding = 1;
	}
	else if (oldProvince->hasBuilding("conscription_center"))
	{
		armyBuilding = 2;
	}

	int productionBuilding = 0;
	if (oldProvince->hasBuilding("workshop"))
	{
		productionBuilding = 1;
	}
	else if (oldProvince->hasBuilding("counting_house"))
	{
		productionBuilding = 2;
	}

	int tradeBuilding = 0;
	if (oldProvince->hasBuilding("marketplace"))
	{
		tradeBuilding = 1;
	}
	else if (oldProvince->hasBuilding("trade_depot"))
	{
		tradeBuilding = 2;
	}
	else if (oldProvince->hasBuilding("stock_exchange"))
	{
		tradeBuilding = 3;
	}

	pts.artisans += 400;
	pts.artisans += static_cast<double>(productionBuilding) * 500;

	pts.soldiers += 100;
	pts.soldiers += (static_cast<double>(manpowerBuilding) + static_cast<double>(armyBuilding)) * 90;
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("quantity_ideas")))
	{
		pts.soldiers *= 2;
	}

	pts.officers += 4 * (static_cast<double>(manpowerBuilding) + static_cast<double>(armyBuilding) + 2.0);
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("quality_ideas")))
	{
		pts.officers += 5;
	}

	pts.clergymen += 65;
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("religious_ideas")))
	{
		pts.clergymen += 10;
	}
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("innovative_ideas")))
	{
		pts.clergymen += 10;
	}
	if (oldProvince->hasBuilding("university"))
	{
		pts.clergymen *= 2;
	}

	pts.bureaucrats += 10;
	pts.bureaucrats += (static_cast<double>(adminBuilding) + static_cast<double>(taxBuilding)) * 4;
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("administrative_ideas")))
	{
		pts.bureaucrats += 10;
	}
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("expansion_ideas")) && oldProvince->wasColonised())
	{
		pts.bureaucrats += 10;
	}

	pts.aristocrats += 14 * (static_cast<double>(tradeBuilding) + 6.0);
	if (oldProvince->hasBuilding("farm_estate") || oldProvince->hasBuilding("plantations"))
	{
		pts.aristocrats *= 2;
	}
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("aristocracy_ideas")))
	{
		pts.aristocrats *= 2;
	}

	if (factories.size() > 0)
	{
		double capsPerFactory = 40 + static_cast<double>(_owner->getNumFactories()) * 2;
		double actualCapitalists = static_cast<double>(factories.size()) * static_cast<double>(_owner->getNumFactories()) * capsPerFactory * demographic.upperRatio;
		pts.capitalists += (10000 * actualCapitalists) / (demographic.upperRatio * newPopulation);

		double actualClerks = 181 * static_cast<double>(factories.size()) * demographic.middleRatio;
		pts.clerks += (10000 * actualClerks) / (demographic.middleRatio * newPopulation);

		double actualCraftsmen = 2639 * static_cast<double>(factories.size()) * demographic.lowerRatio;
		pts.craftsmen += (10000 * actualCraftsmen) / (demographic.lowerRatio * newPopulation);
	}

	return pts;
}


void V2Province::createPops(
	const V2Demographic& demographic,
	double popWeightRatio,
	const V2Country* _owner,
	int popConversionAlgorithm,
	const std::map<std::string, std::shared_ptr<EU4::Country>>& theEU4Countries,
	const mappers::ProvinceMapper& provinceMapper
) {
	const EU4::Province*	oldProvince = demographic.oldProvince;
	auto oldCountry = demographic.oldCountry;

	long newPopulation = 0;
	double lifeRatingMod = (static_cast<double>(this->lifeRating) - 30.0) / 200.0;
	double devpushMod = oldProvince->getDevDelta() / 100.0;
	double weightMod = oldProvince->getModifierWeight() / 100.0;
	double shapeMod = theConfiguration.getPopShapingFactor() / 100.0;
	double provinceDevModifier = 1 + (lifeRatingMod + devpushMod + weightMod) * shapeMod;

	switch (theConfiguration.getPopShaping()) {
	case Configuration::POPSHAPES::Vanilla:
		newPopulation = oldPopulation;
		break;

	case Configuration::POPSHAPES::PopShaping:
		if (spentProvinceModifier != 0)
		{
			// We have already created pops here using another EU4 province. We need to compound the push and weight modifiers.
			provinceDevModifier += spentProvinceModifier;
		}
		spentProvinceModifier += (devpushMod + weightMod) * shapeMod;

		newPopulation = static_cast<long>(oldPopulation * provinceDevModifier);
		break;

	case Configuration::POPSHAPES::Extreme:
		newPopulation = static_cast<long>((static_cast<double>(this->lifeRating) / 10)* popWeightRatio* oldProvince->getTotalWeight());

		auto Vic2Provinces = provinceMapper.getVic2ProvinceNumbers(srcProvince->getNum());
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

		newPopulation = oldPopulation + static_cast<long>((newPopulation - oldPopulation) * (theConfiguration.getPopShapingFactor() / 100.0));
		break;
	}

	pop_points pts;
	switch (popConversionAlgorithm)
	{
	case 1:
		pts = getPopPoints_1(demographic, newPopulation, _owner, theEU4Countries);
		break;
	case 2:
		pts = getPopPoints_2(demographic, newPopulation, _owner, theEU4Countries);
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

	int farmers = static_cast<int>(demographic.lowerRatio * newPopulation + 0.5);
	if (slaveProportion > 0.0)
	{
		int size = static_cast<int>(demographic.lowerRatio * newPopulation * slaveProportion);
		farmers -= size;
		V2::Pop* slavesPop = new V2::Pop("slaves", size, demographic.slaveCulture, demographic.religion);
		pops.push_back(slavesPop);
	}
	if (pts.soldiers > 0)
	{
		int size = static_cast<int>(demographic.lowerRatio * newPopulation * (pts.soldiers / 10000) + 0.5);
		farmers -= size;
		V2::Pop* soldiersPop = new V2::Pop("soldiers", size, demographic.culture, demographic.religion);
		pops.push_back(soldiersPop);
	}
	if (pts.craftsmen > 0)
	{
		int size = static_cast<int>(demographic.lowerRatio * newPopulation * (pts.craftsmen / 10000) + 0.5);
		farmers -= size;
		V2::Pop* craftsmenPop = new V2::Pop("craftsmen", size, demographic.culture, demographic.religion);
		pops.push_back(craftsmenPop);
	}
	if (pts.artisans > 0)
	{
		int size = static_cast<int>(demographic.middleRatio * newPopulation * (pts.artisans / 10000) + 0.5);
		farmers -= size;
		V2::Pop* artisansPop = new V2::Pop("artisans", size, demographic.culture, demographic.religion);
		pops.push_back(artisansPop);
	}
	if (pts.clergymen > 0)
	{
		int size = static_cast<int>(demographic.middleRatio * newPopulation * (pts.clergymen / 10000) + 0.5);
		farmers -= size;
		V2::Pop* clergymenPop = new V2::Pop("clergymen", size, demographic.culture, demographic.religion);
		pops.push_back(clergymenPop);
	}
	if (pts.clerks > 0)
	{
		int size = static_cast<int>(demographic.middleRatio * newPopulation * (pts.clerks / 10000) + 0.5);
		farmers -= size;
		V2::Pop* clerksPop = new V2::Pop("clerks", size, demographic.culture, demographic.religion);
		pops.push_back(clerksPop);
	}
	if (pts.bureaucrats > 0)
	{
		int size = static_cast<int>(demographic.middleRatio * newPopulation * (pts.bureaucrats / 10000) + 0.5);
		farmers -= size;
		V2::Pop* bureaucratsPop = new V2::Pop("bureaucrats", size, demographic.culture, demographic.religion);
		pops.push_back(bureaucratsPop);
	}
	if (pts.officers > 0)
	{
		int size = static_cast<int>(demographic.middleRatio * newPopulation * (pts.officers / 10000) + 0.5);
		farmers -= size;
		V2::Pop* officersPop = new V2::Pop("officers", size, demographic.culture, demographic.religion);
		pops.push_back(officersPop);
	}
	if (pts.capitalists > 0)
	{
		int size = static_cast<int>(demographic.upperRatio * newPopulation * (pts.capitalists / 10000) + 0.5);
		farmers -= size;
		V2::Pop* capitalistsPop = new V2::Pop("capitalists", size, demographic.culture, demographic.religion);
		pops.push_back(capitalistsPop);
	}
	if (pts.aristocrats > 0)
	{
		int size = static_cast<int>(demographic.upperRatio * newPopulation * (pts.aristocrats / 10000) + 0.5);
		farmers -= size;
		V2::Pop* aristocratsPop = new V2::Pop("aristocrats", size, demographic.culture, demographic.religion);
		pops.push_back(aristocratsPop);
	}

	V2::Pop* farmersPop = new V2::Pop("farmers", farmers, demographic.culture, demographic.religion);
	pops.push_back(farmersPop);

	LOG(LogLevel::Info) << "Name: " << this->getSrcProvince()->getName() << " demographics.upperRatio: " << demographic.upperRatio
		<< " demographics.middleRatio: " << demographic.middleRatio << " demographics.lowerRatio: " << demographic.lowerRatio
		<< " newPopulation: " << newPopulation << " farmer: " << farmers	<< " total: " << newPopulation;
}


void V2Province::combinePops()
{
	std::vector<V2::Pop*> trashPops;
	for (std::vector<V2::Pop*>::iterator lhs = pops.begin(); lhs != pops.end(); ++lhs)
	{
		std::vector<V2::Pop*>::iterator rhs = lhs;
		for (++rhs; rhs != pops.end(); ++rhs)
		{
			if ((*lhs)->combine(**rhs))
			{
				trashPops.push_back(*rhs);
			}
			if ((*rhs)->getSize() < 1)
			{
				trashPops.push_back(*rhs);
			}
		}
	}

	std::vector<V2::Pop*> consolidatedPops;
	for (std::vector<V2::Pop*>::iterator itr = pops.begin(); itr != pops.end(); ++itr)
	{
		bool isTrashed = false;
		for (std::vector<V2::Pop*>::iterator titr = trashPops.begin(); titr != trashPops.end(); ++titr)
		{
			if (*itr == *titr)
			{
				isTrashed = true;
			}
		}
		if (!isTrashed)
		{
			consolidatedPops.push_back(*itr);
		}
	}
	pops.swap(consolidatedPops);
}








std::vector<V2::Pop*> V2Province::getPops(const std::string& type) const
{
	std::vector<V2::Pop*> retval;
	for (std::vector<V2::Pop*>::const_iterator itr = pops.begin(); itr != pops.end(); ++itr)
	{
		if (type == "*" || (*itr)->getType() == type)
			retval.push_back(*itr);
	}
	return retval;
}


static bool PopSortBySizePredicate(const V2::Pop* pop1, const V2::Pop* pop2)
{
	return (pop1->getSize() > pop2->getSize());
}


// V2 requires 1000 for the first regiment and 3000 thereafter
// we require an extra 1/30 to stabilize the start of the game
static int getRequiredPopForRegimentCount(int count)
{
	if (count == 0) return 0;
	return (1033 + (count - 1) * 3100);
}


// pick a soldier pop to use for an army.  prefer larger pops to smaller ones, and grow only if necessary.
V2::Pop* V2Province::getSoldierPopForArmy(bool force)
{
	std::vector<V2::Pop*> spops = getPops("soldiers");
	if (spops.size() == 0)
		return NULL; // no soldier pops

	std::sort(spops.begin(), spops.end(), PopSortBySizePredicate);
	// try largest to smallest, without growing
	for (std::vector<V2::Pop*>::iterator itr = spops.begin(); itr != spops.end(); ++itr)
	{
		int growBy = getRequiredPopForRegimentCount((*itr)->getSupportedRegimentCount() + 1) - (*itr)->getSize();
		if (growBy <= 0)
		{
			if (growSoldierPop(*itr)) // won't actually grow, but necessary to increment supported regiment count
			{
				return *itr;
			}
		}
	}
	// try largest to smallest, trying to grow
	for (std::vector<V2::Pop*>::iterator itr = spops.begin(); itr != spops.end(); ++itr)
	{
		if (growSoldierPop(*itr))
		{
			return *itr;
		}
	}

	// no suitable pops
	if (force)
	{
		return spops[0];
	}
	else
	{
		return NULL;
	}
}


bool V2Province::growSoldierPop(V2::Pop* pop)
{
	int growBy = getRequiredPopForRegimentCount(pop->getSupportedRegimentCount() + 1) - pop->getSize();
	if (growBy > 0)
	{
		// gotta grow; find a same-culture same-religion farmer/laborer to pull from
		int provincePop = getTotalPopulation();
		bool foundSourcePop = false;
		for (std::vector<V2::Pop*>::iterator isrc = pops.begin(); isrc != pops.end(); ++isrc)
		{
			if ((*isrc)->getType() == "farmers" || (*isrc)->getType() == "labourers")
			{
				if ((*isrc)->getCulture() == pop->getCulture() && (*isrc)->getReligion() == pop->getReligion())
				{
					// don't let the farmer/labourer shrink beneath 10% of the province population
					if (static_cast<double>((*isrc)->getSize()) - growBy > provincePop * 0.10)
					{
						(*isrc)->changeSize(-growBy);
						pop->changeSize(growBy);
						foundSourcePop = true;
						break;
					}
				}
			}
		}
		if (!foundSourcePop)
		{
			return false;
		}
	}
	pop->incrementSupportedRegimentCount();
	return true;
}


std::pair<int, int> V2Province::getAvailableSoldierCapacity() const
{
	int soldierCap = 0;
	int draftCap = 0;
	int provincePop = getTotalPopulation();
	for (std::vector<V2::Pop*>::const_iterator itr = pops.begin(); itr != pops.end(); ++itr)
	{
		if ((*itr)->getType() == "soldiers")
		{
			// unused capacity is the size of the pop minus the capacity already used, or 0, if it's already overdrawn
			soldierCap += std::max((*itr)->getSize() - getRequiredPopForRegimentCount((*itr)->getSupportedRegimentCount()), 0);
		}
		else if ((*itr)->getType() == "farmers" || (*itr)->getType() == "labourers")
		{
			// unused capacity is the size of the pop in excess of 10% of the province pop, or 0, if it's already too small
			draftCap += std::max((*itr)->getSize() - int(0.10 * provincePop), 0);
		}
	}
	return std::pair<int, int>(soldierCap, draftCap);
}


std::string V2Province::getRegimentName(V2::REGIMENTTYPE chosenType)
{
	std::stringstream str;
	str << ++unitNameCount[chosenType] << CardinalToOrdinal(unitNameCount[chosenType]); // 1st, 2nd, etc
	str << " " << name << " "; // Hamburg, Lyon, etc
	switch (chosenType)
	{
	case V2::REGIMENTTYPE::irregular:
		str << "Irregulars";
		break;
	case V2::REGIMENTTYPE::infantry:
		str << "Infantry";
		break;
	case V2::REGIMENTTYPE::cavalry:
		str << "Cavalry";
		break;
	case V2::REGIMENTTYPE::artillery:
		str << "Artillery";
		break;
	case V2::REGIMENTTYPE::manowar:
		str << "Man'o'war";
		break;
	case V2::REGIMENTTYPE::frigate:
		str << "Frigate";
		break;
	case V2::REGIMENTTYPE::clipper_transport:
		str << "Clipper Transport";
		break;
	}
	return str.str();
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