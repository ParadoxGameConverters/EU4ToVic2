#include "V2Province.h"
#include "Factory/V2Factory.h"
#include "CardinalToOrdinal.h"
#include "Log.h"
#include "Object.h"
#include "OSCompatibilityLayer.h"
#include "ParadoxParser8859_15.h"
#include "../EU4World/World.h"
#include "../EU4World/Provinces/EU4Province.h"
#include "../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "V2Pop.h"
#include "V2Country.h"
#include "Factory/V2Factory.h"
#include <algorithm>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdio.h>
#include "ParserHelpers.h"


V2Province::V2Province(std::string _filename)
{
	filename = _filename;
	for (int i = 0; i < static_cast<int>(EU4::REGIMENTCATEGORY::num_reg_categories); ++i)
	{
		unitNameCount[i] = 0;
	}

	registerKeyword(std::regex("owner"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString ownerStr(theStream);
			owner = ownerStr.getString();
		});
	registerKeyword(std::regex("controller"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString controllerStr(theStream);
			controller = controllerStr.getString();
		});
	registerKeyword(std::regex("add_core"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString coreStr(theStream);
			cores.push_back(coreStr.getString());
		});
	registerKeyword(std::regex("trade_goods"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString rgoTypeStr(theStream);
			rgoType = rgoTypeStr.getString();
		});
	registerKeyword(std::regex("life_rating"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt lifeRatingInt(theStream);
			lifeRating = lifeRatingInt.getInt();
		});
	registerKeyword(std::regex("terrain"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString terrainStr(theStream);
			terrain = terrainStr.getString();
		});
	registerKeyword(std::regex("colonial"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt colonialInt(theStream);
			colonial = colonialInt.getInt();
		});
	registerKeyword(std::regex("colony"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt colonyInt(theStream);
			colonyLevel = colonyInt.getInt();
		});
	registerKeyword(std::regex("naval_base"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt navalBaseLevelInt(theStream);
			navalBaseLevel = navalBaseLevelInt.getInt();
		});
	registerKeyword(std::regex("fort"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt fortLevelInt(theStream);
			fortLevel = fortLevelInt.getInt();
		});
	registerKeyword(std::regex("railroad"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt railLevelInt(theStream);
			railLevel = railLevelInt.getInt();
		});
	registerKeyword(std::regex("is_slave"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString ignoredStr(theStream);
			slaveState = true;
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	int slash = filename.find_last_of("/");
	int numDigits = filename.find_first_of("-") - slash - 2;
	std::string temp = filename.substr(slash + 1, numDigits);
	num = atoi(temp.c_str());

	if (Utils::DoesFileExist("./blankMod/output/history/provinces" + filename))
	{
		parseFile("./blankMod/output/history/provinces" + filename);
	}
	else
	{
		parseFile(theConfiguration.getVic2Path() + "/history/provinces" + filename);
	}
}


void V2Province::output() const
{
	int lastSlash = filename.find_last_of('/');
	std::string path = filename.substr(0, lastSlash);
	Utils::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/history/provinces" + path);

	std::ofstream output("output/" + theConfiguration.getOutputName() + "/history/provinces" + filename);
	if (!output.is_open())
	{
		LOG(LogLevel::Error) << "Could not create province history file output/" << theConfiguration.getOutputName() << "/history/provinces/" << filename << " - " << Utils::GetLastErrorString();
		exit(-1);
	}
	if (owner != "")
	{
		output << "owner=" << owner << "\n";
		output << "controller=" << controller << "\n";
	}
	for (auto core: cores)
	{
		output << "add_core=" << core << "\n";
	}
	if (inHRE)
	{
		output << "add_core=HRE\n";
	}
	if (rgoType != "")
	{
		output << "trade_goods = " << rgoType << "\n";
	}
	if (lifeRating > 0)
	{
		output << "life_rating = " << lifeRating << "\n";
	}
	if (terrain != "")
	{
		output << "terrain = " << terrain << "\n";
	}
	if (colonial > 0)
	{
		output << "colonial=" << colonial << "\n";
	}
	if (navalBaseLevel > 0)
	{
		output << "naval_base = " << navalBaseLevel << "\n";
	}
	if (fortLevel > 0)
	{
		output << "fort = " << fortLevel << "\n";
	}
	if (railLevel > 0)
	{
		output << "railroad = " << railLevel << "\n";
	}
	if (slaveState)
	{
		output << "is_slave = yes\n";
	}
	for (auto factory: factories)
	{
		output << factory.second;
	}
	output.close();
}


void V2Province::outputPops(FILE* output) const
{
	if (resettable && (theConfiguration.getResetProvinces() == "yes"))
	{
		fprintf(output, "%d = {\n", num);
		if (oldPops.size() > 0)
		{
			for (unsigned int i = 0; i < oldPops.size(); i++)
			{
				oldPops[i]->output(output);
				fprintf(output, "\n");
			}
			fprintf(output, "}\n");
		}
	}
	else
	{
		if (pops.size() > 0)
		{
			fprintf(output, "%d = {\n", num);
			for (auto i : pops)
			{
				i->output(output);
				fprintf(output, "\n");
			}
			fprintf(output, "}\n");
		}
		else if (oldPops.size() > 0)
		{
			fprintf(output, "%d = {\n", num);
			for (unsigned int i = 0; i < oldPops.size(); i++)
			{
				oldPops[i]->output(output);
				fprintf(output, "\n");
			}
			fprintf(output, "}\n");
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


void V2Province::convertFromOldProvince(
	const EU4::Religions& allReligions,
	const EU4::Province* oldProvince,
	const std::map<std::string, std::shared_ptr<EU4::Country>>& theEU4Countries
) {
	srcProvince = oldProvince;
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
	auto oldCountry = theEU4Countries.find(oldProvince->getOwnerString());
	if (oldCountry != theEU4Countries.end())
	{
		originallyInfidel = oldProvince->wasInfidelConquest(oldCountry->second->getReligion(), allReligions);
	}
}


void V2Province::determineColonial()
{
	if (territorialCore && (colonial == 0))
	{
		colonial = 2;
	}
}


void V2Province::addCore(string newCore)
{
	// only add if unique, and not a territorial core/colony of the current owner
	if ((find(cores.begin(), cores.end(), newCore) == cores.end()) && !((newCore.compare(owner) == 0) && territorialCore))
	{
		cores.push_back(newCore);
	}
}


void V2Province::addOldPop(const V2Pop* oldPop)
{
	oldPops.push_back(oldPop);
	oldPopulation += oldPop->getSize();
}


void V2Province::addMinorityPop(V2Pop* minorityPop)
{
	minorityPops.push_back(minorityPop);
}


void V2Province::doCreatePops(
	double popWeightRatio,
	V2Country* _owner,
	int popConversionAlgorithm,
	const std::map<std::string, std::shared_ptr<EU4::Country>>& theEU4Countries,
	const mappers::ProvinceMapper& provinceMapper
) {
	// convert pops
	for (vector<V2Demographic>::const_iterator itr = demographics.begin(); itr != demographics.end(); ++itr)
	{
		createPops(*itr, popWeightRatio, _owner, popConversionAlgorithm, theEU4Countries, provinceMapper);
	}
	combinePops();

	// organize pops for adding minorities
	map<string, int>					totals;
	map<string, vector<V2Pop*>>	thePops;
	for (auto popItr : pops)
	{
		string type = popItr->getType();

		auto totalsItr = totals.find(type);
		if (totalsItr == totals.end())
		{
			totals.insert(make_pair(type, popItr->getSize()));
		}
		else
		{
			totalsItr->second += popItr->getSize();
		}

		auto thePopsItr = thePops.find(type);
		if (thePopsItr == thePops.end())
		{
			vector<V2Pop*> newVector;
			newVector.push_back(popItr);
			thePops.insert(make_pair(type, newVector));
		}
		else
		{
			thePopsItr->second.push_back(popItr);
		}
	}

	// decrease non-minority pops and create the minorities
	vector<V2Pop*> actualMinorities;
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
				string newCulture = minorityItr->getCulture();
				string newReligion = minorityItr->getReligion();
				if (newCulture == "")
				{
					newCulture = popsItr->getCulture();
				}
				if (newReligion == "")
				{
					newReligion = popsItr->getReligion();
				}

				V2Pop* newMinority = new V2Pop(minorityItr->getType(), static_cast<int>(1.0 * popsItr->getSize() / totalTypePopulation * minorityItr->getSize() + 0.5), newCulture, newReligion);
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
	pts.artisans += productionBuilding * 125;

	pts.soldiers += 100;
	pts.soldiers += armyBuilding * 45;
	if ((oldCountryTag != "") && (oldCountry->hasNationalIdea("quantity_ideas") != -1))
	{
		pts.soldiers *= 2;
	}

	pts.officers += 2 * (armyBuilding + 2);
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("quality_ideas") != -1))
	{
		pts.officers += (oldCountry->hasNationalIdea("quality_ideas") / 2);
	}

	pts.clergymen += 95;
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("religious_ideas") != -1))
	{
		pts.clergymen += oldCountry->hasNationalIdea("religious_ideas");
	}
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("innovative_ideas") != -1))
	{
		pts.clergymen += oldCountry->hasNationalIdea("innovative_ideas");
	}

	pts.bureaucrats += 10;
	pts.bureaucrats += govBuilding * 2;
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("administrative_ideas") != -1))
	{
		pts.bureaucrats += oldCountry->hasNationalIdea("administrative_ideas");
	}
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("expansion_ideas") != -1) && oldProvince->wasColonised())
	{
		pts.bureaucrats += oldCountry->hasNationalIdea("expansion_ideas");
	}

	pts.aristocrats += 7 * (tradeBuilding + 11);
	if (oldProvince->hasBuilding("farm_estate") ||
		oldProvince->hasBuilding("plantations")
		)
	{
		pts.aristocrats *= 2;
	}
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("aristocracy_ideas") != -1))
	{
		pts.aristocrats *= 2;
	}

	if (factories.size() > 0)
	{
		double capsPerFactory = 40 + _owner->getNumFactories() * 2;
		double actualCapitalists = factories.size() * _owner->getNumFactories() * capsPerFactory * demographic.upperRatio;
		pts.capitalists += (10000 * actualCapitalists) / (demographic.upperRatio * newPopulation);

		double actualClerks = 181 * factories.size() * demographic.middleRatio;
		pts.clerks += (10000 * actualClerks) / (demographic.middleRatio * newPopulation);

		double actualCraftsmen = 2639 * factories.size() * demographic.lowerRatio;
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
	pts.artisans += productionBuilding * 500;

	pts.soldiers += 100;
	pts.soldiers += (manpowerBuilding + armyBuilding) * 90;
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("quantity_ideas") != -1))
	{
		pts.soldiers *= 2;
	}

	pts.officers += 4 * (manpowerBuilding + armyBuilding + 2);
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("quality_ideas") != -1))
	{
		pts.officers += (oldCountry->hasNationalIdea("quality_ideas") / 2);
	}

	pts.clergymen += 65;
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("religious_ideas") != -1))
	{
		pts.clergymen += oldCountry->hasNationalIdea("religious_ideas");
	}
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("innovative_ideas") != -1))
	{
		pts.clergymen += oldCountry->hasNationalIdea("innovative_ideas");
	}
	if (oldProvince->hasBuilding("university"))
	{
		pts.clergymen *= 2;
	}

	pts.bureaucrats += 10;
	pts.bureaucrats += (adminBuilding + taxBuilding) * 4;
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("administrative_ideas") != -1))
	{
		pts.bureaucrats += oldCountry->hasNationalIdea("administrative_ideas");
	}
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("expansion_ideas") != -1) && oldProvince->wasColonised())
	{
		pts.bureaucrats += oldCountry->hasNationalIdea("expansion_ideas");
	}

	pts.aristocrats += 14 * (tradeBuilding + 6);
	if (oldProvince->hasBuilding("farm_estate") || oldProvince->hasBuilding("plantations"))
	{
		pts.aristocrats *= 2;
	}
	if ((oldCountry != NULL) && (oldCountry->hasNationalIdea("aristocracy_ideas") != -1))
	{
		pts.aristocrats *= 2;
	}

	if (factories.size() > 0)
	{
		double capsPerFactory = 40 + _owner->getNumFactories() * 2;
		double actualCapitalists = factories.size() * _owner->getNumFactories() * capsPerFactory * demographic.upperRatio;
		pts.capitalists += (10000 * actualCapitalists) / (demographic.upperRatio * newPopulation);

		double actualClerks = 181 * factories.size() * demographic.middleRatio;
		pts.clerks += (10000 * actualClerks) / (demographic.middleRatio * newPopulation);

		double actualCraftsmen = 2639 * factories.size() * demographic.lowerRatio;
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
		V2Pop* slavesPop = new V2Pop("slaves", size, demographic.slaveCulture, demographic.religion);
		pops.push_back(slavesPop);
	}
	if (pts.soldiers > 0)
	{
		int size = static_cast<int>(demographic.lowerRatio * newPopulation * (pts.soldiers / 10000) + 0.5);
		farmers -= size;
		V2Pop* soldiersPop = new V2Pop("soldiers", size, demographic.culture, demographic.religion);
		pops.push_back(soldiersPop);
	}
	if (pts.craftsmen > 0)
	{
		int size = static_cast<int>(demographic.lowerRatio * newPopulation * (pts.craftsmen / 10000) + 0.5);
		farmers -= size;
		V2Pop* craftsmenPop = new V2Pop("craftsmen", size, demographic.culture, demographic.religion);
		pops.push_back(craftsmenPop);
	}
	if (pts.artisans > 0)
	{
		int size = static_cast<int>(demographic.middleRatio * newPopulation * (pts.artisans / 10000) + 0.5);
		farmers -= size;
		V2Pop* artisansPop = new V2Pop("artisans", size, demographic.culture, demographic.religion);
		pops.push_back(artisansPop);
	}
	if (pts.clergymen > 0)
	{
		int size = static_cast<int>(demographic.middleRatio * newPopulation * (pts.clergymen / 10000) + 0.5);
		farmers -= size;
		V2Pop* clergymenPop = new V2Pop("clergymen", size, demographic.culture, demographic.religion);
		pops.push_back(clergymenPop);
	}
	if (pts.clerks > 0)
	{
		int size = static_cast<int>(demographic.middleRatio * newPopulation * (pts.clerks / 10000) + 0.5);
		farmers -= size;
		V2Pop* clerksPop = new V2Pop("clerks", size, demographic.culture, demographic.religion);
		pops.push_back(clerksPop);
	}
	if (pts.bureaucrats > 0)
	{
		int size = static_cast<int>(demographic.middleRatio * newPopulation * (pts.bureaucrats / 10000) + 0.5);
		farmers -= size;
		V2Pop* bureaucratsPop = new V2Pop("bureaucrats", size, demographic.culture, demographic.religion);
		pops.push_back(bureaucratsPop);
	}
	if (pts.officers > 0)
	{
		int size = static_cast<int>(demographic.middleRatio * newPopulation * (pts.officers / 10000) + 0.5);
		farmers -= size;
		V2Pop* officersPop = new V2Pop("officers", size, demographic.culture, demographic.religion);
		pops.push_back(officersPop);
	}
	if (pts.capitalists > 0)
	{
		int size = static_cast<int>(demographic.upperRatio * newPopulation * (pts.capitalists / 10000) + 0.5);
		farmers -= size;
		V2Pop* capitalistsPop = new V2Pop("capitalists", size, demographic.culture, demographic.religion);
		pops.push_back(capitalistsPop);
	}
	if (pts.aristocrats > 0)
	{
		int size = static_cast<int>(demographic.upperRatio * newPopulation * (pts.aristocrats / 10000) + 0.5);
		farmers -= size;
		V2Pop* aristocratsPop = new V2Pop("aristocrats", size, demographic.culture, demographic.religion);
		pops.push_back(aristocratsPop);
	}

	V2Pop* farmersPop = new V2Pop("farmers", farmers, demographic.culture, demographic.religion);
	pops.push_back(farmersPop);

	/*LOG(LogLevel::Info) << "Name: " << this->getSrcProvince()->getName() << " demographics.upperRatio: " << demographic.upperRatio
		<< " demographics.middleRatio: " << demographic.middleRatio << " demographics.lowerRatio: " << demographic.lowerRatio
		<< " newPopulation: " << newPopulation << " farmer: " << farmers	<< " total: " << newPopulation;*/
}


void V2Province::combinePops()
{
	vector<V2Pop*> trashPops;
	for (vector<V2Pop*>::iterator lhs = pops.begin(); lhs != pops.end(); ++lhs)
	{
		vector<V2Pop*>::iterator rhs = lhs;
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

	vector<V2Pop*> consolidatedPops;
	for (vector<V2Pop*>::iterator itr = pops.begin(); itr != pops.end(); ++itr)
	{
		bool isTrashed = false;
		for (vector<V2Pop*>::iterator titr = trashPops.begin(); titr != trashPops.end(); ++titr)
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


void V2Province::addFactory(const V2Factory& factory)
{
	map<string, V2Factory>::iterator itr = factories.find(factory.getTypeName());
	if (itr == factories.end())
	{
		factories.insert(std::make_pair(factory.getTypeName(), factory));
	}
	else
	{
		itr->second.increaseLevel();
	}
}


void V2Province::addPopDemographic(V2Demographic d)
{
	bool combined = false;
	for (auto itr = demographics.begin(); itr != demographics.end(); itr++)
	{
		if ((itr->culture == d.culture) && (itr->religion == d.religion))
		{
			combined = true;
			itr->upperRatio += d.upperRatio;
			itr->middleRatio += d.middleRatio;
			itr->lowerRatio += d.lowerRatio;
		}
	}
	if (!combined)
	{
		demographics.push_back(d);
	}
}


int V2Province::getTotalPopulation() const
{
	int total = 0;
	for (vector<V2Pop*>::const_iterator itr = pops.begin(); itr != pops.end(); ++itr)
	{
		total += (*itr)->getSize();
	}
	return total;
}


vector<V2Pop*> V2Province::getPops(string type) const
{
	vector<V2Pop*> retval;
	for (vector<V2Pop*>::const_iterator itr = pops.begin(); itr != pops.end(); ++itr)
	{
		if (type == "*" || (*itr)->getType() == type)
			retval.push_back(*itr);
	}
	return retval;
}


static bool PopSortBySizePredicate(const V2Pop* pop1, const V2Pop* pop2)
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
V2Pop* V2Province::getSoldierPopForArmy(bool force)
{
	vector<V2Pop*> spops = getPops("soldiers");
	if (spops.size() == 0)
		return NULL; // no soldier pops

	sort(spops.begin(), spops.end(), PopSortBySizePredicate);
	// try largest to smallest, without growing
	for (vector<V2Pop*>::iterator itr = spops.begin(); itr != spops.end(); ++itr)
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
	for (vector<V2Pop*>::iterator itr = spops.begin(); itr != spops.end(); ++itr)
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


bool V2Province::growSoldierPop(V2Pop* pop)
{
	int growBy = getRequiredPopForRegimentCount(pop->getSupportedRegimentCount() + 1) - pop->getSize();
	if (growBy > 0)
	{
		// gotta grow; find a same-culture same-religion farmer/laborer to pull from
		int provincePop = getTotalPopulation();
		bool foundSourcePop = false;
		for (vector<V2Pop*>::iterator isrc = pops.begin(); isrc != pops.end(); ++isrc)
		{
			if ((*isrc)->getType() == "farmers" || (*isrc)->getType() == "labourers")
			{
				if ((*isrc)->getCulture() == pop->getCulture() && (*isrc)->getReligion() == pop->getReligion())
				{
					// don't let the farmer/labourer shrink beneath 10% of the province population
					if ((*isrc)->getSize() - growBy > provincePop * 0.10)
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


pair<int, int> V2Province::getAvailableSoldierCapacity() const
{
	int soldierCap = 0;
	int draftCap = 0;
	int provincePop = getTotalPopulation();
	for (vector<V2Pop*>::const_iterator itr = pops.begin(); itr != pops.end(); ++itr)
	{
		if ((*itr)->getType() == "soldiers")
		{
			// unused capacity is the size of the pop minus the capacity already used, or 0, if it's already overdrawn
			soldierCap += max((*itr)->getSize() - getRequiredPopForRegimentCount((*itr)->getSupportedRegimentCount()), 0);
		}
		else if ((*itr)->getType() == "farmers" || (*itr)->getType() == "labourers")
		{
			// unused capacity is the size of the pop in excess of 10% of the province pop, or 0, if it's already too small
			draftCap += max((*itr)->getSize() - int(0.10 * provincePop), 0);
		}
	}
	return pair<int, int>(soldierCap, draftCap);
}


string V2Province::getRegimentName(EU4::REGIMENTCATEGORY rc)
{
	// galleys turn into light ships; count and name them identically
	if (rc == EU4::REGIMENTCATEGORY::galley)
		rc = EU4::REGIMENTCATEGORY::light_ship;

	stringstream str;
	str << ++unitNameCount[static_cast<int>(rc)] << CardinalToOrdinal(unitNameCount[static_cast<int>(rc)]); // 1st, 2nd, etc
	str << " " << name << " "; // Hamburg, Lyon, etc
	switch (rc)
	{
	case EU4::REGIMENTCATEGORY::artillery:
		str << "Artillery";
		break;
	case EU4::REGIMENTCATEGORY::infantry:
		str << "Infantry";
		break;
	case EU4::REGIMENTCATEGORY::cavalry:
		str << "Cavalry";
		break;
	case EU4::REGIMENTCATEGORY::heavy_ship:
		str << "Man'o'war";
		break;
	case EU4::REGIMENTCATEGORY::light_ship:
		str << "Frigate";
		break;
	case EU4::REGIMENTCATEGORY::transport:
		str << "Clipper Transport";
		break;
	}
	return str.str();
}


bool V2Province::hasCulture(string culture, float percentOfPopulation) const
{
	int culturePops = 0;
	for (vector<V2Pop*>::const_iterator itr = pops.begin(); itr != pops.end(); ++itr)
	{
		if ((*itr)->getCulture() == culture)
		{
			culturePops += (*itr)->getSize();
		}
	}

	return ((float)culturePops / getTotalPopulation()) >= percentOfPopulation;
}


vector<string> V2Province::getCulturesOverThreshold(float percentOfPopulation) const
{
	int totalPopulation = getTotalPopulation();

	map<string, double> cultureAmounts;
	for (auto pop : pops)
	{
		auto cultureAmount = cultureAmounts.find(pop->getCulture());
		if (cultureAmount == cultureAmounts.end())
		{
			cultureAmounts.insert(make_pair(pop->getCulture(), 0.0f));
			cultureAmount = cultureAmounts.find(pop->getCulture());
		}
		if (totalPopulation > 0)
		{
			cultureAmount->second += 1.0 * pop->getSize() / totalPopulation;
		}
	}

	vector<string> culturesOverThreshold;
	for (auto cultureAmount : cultureAmounts)
	{
		if (cultureAmount.second >= percentOfPopulation)
		{
			culturesOverThreshold.push_back(cultureAmount.first);
		}
	}

	return culturesOverThreshold;
}
