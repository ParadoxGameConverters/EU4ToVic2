#include "V2World.h"
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <list>
#include <queue>
#include <cmath>
#include <cfloat>
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "../Configuration.h"
#include "../EU4World/Diplomacy/EU4Diplomacy.h"
#include "../EU4World/World.h"
#include "../EU4World/Provinces/EU4Province.h"
#include "../Helpers/TechValues.h"
#include "../Mappers/CultureMapper/CultureMapper.h"
#include "State/State.h"
#include "Pop/Pop.h"
#include "V2Country.h"
#include "Flags/Flags.h"
#include "../Mappers/Pops/PopMapper.h"
#include "../EU4World/Country/EU4Country.h"
#include "../Mappers/IdeaEffects/IdeaEffectMapper.h"
#include "../Mappers/TechGroups/TechGroupsMapper.h"

V2::V2World::V2World(const EU4::World& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper, const mappers::TechGroupsMapper& techGroupsMapper)
{
	LOG(LogLevel::Info) << "*** Hello Vicky 2, creating world. ***";
	LOG(LogLevel::Info) << "-> Importing Provinces";
	importProvinces();
	LOG(LogLevel::Info) << "-> Importing Vanilla Pops";
	importDefaultPops();
	//logPopsByCountry();
	LOG(LogLevel::Info) << "-> Determining Coastal Provinces";
	findCoastalProvinces();
	LOG(LogLevel::Info) << "-> Importing Potential Countries";
	importPotentialCountries();
	isRandomWorld = sourceWorld.isRandomWorld();

	LOG(LogLevel::Info) << "-> Checking All Land Provinces Mapped (and may kraken take the rest)";
	sourceWorld.checkAllProvincesMapped(provinceMapper);

	LOG(LogLevel::Info) << "-> Loading Country Mapping Rules";
	countryMapper.createMappings(sourceWorld, potentialCountries, provinceMapper);

	LOG(LogLevel::Info) << "-> Loading Culture Mapping Rules";
	initializeCultureMappers(sourceWorld);
	LOG(LogLevel::Info) << "-> Checking all Cultures Mapped";
	sourceWorld.checkAllEU4CulturesMapped(cultureMapper);
	LOG(LogLevel::Info) << "-> Checking all Religions Mapped";
	sourceWorld.checkAllEU4ReligionsMapped(religionMapper);

	LOG(LogLevel::Info) << "-> Convering Countries";
	convertCountries(sourceWorld, ideaEffectMapper);
	LOG(LogLevel::Info) << "-> Converting Provinces";
	convertProvinces(sourceWorld);
	LOG(LogLevel::Info) << "-> Converting Diplomacy";
	diplomacy.convertDiplomacy(sourceWorld.getDiplomaticAgreements(), countryMapper, countries);
	LOG(LogLevel::Info) << "-> Setting Up Colonies";
	setupColonies();
	LOG(LogLevel::Info) << "-> Setting Up States";
	setupStates();
	LOG(LogLevel::Info) << "-> Generating Unciv Reforms";
	convertUncivReforms(sourceWorld, techGroupsMapper);
	LOG(LogLevel::Info) << "-> Converting Technology Levels";
	convertTechs(sourceWorld);
	LOG(LogLevel::Info) << "-> Distributing Factories";
	allocateFactories(sourceWorld);
	LOG(LogLevel::Info) << "-> Distributing Pops";
	setupPops(sourceWorld);
	LOG(LogLevel::Info) << "-> Merging Nations";
	addUnions();
	LOG(LogLevel::Info) << "-> Converting Armies and Navies";
	convertArmies(sourceWorld);

	LOG(LogLevel::Info) << "---> Le Dump <---";
	auto potentialGPs = countCivilizedNations();
	output(potentialGPs);
	LOG(LogLevel::Info) << "*** Goodbye, Vicky 2, and godspeed. ***";
}

void V2::V2World::shuffleRgos()
{
	if (bucketShuffler.empty())
	{
		LOG(LogLevel::Warning) << "No valid buckets defined, skipping RGO randomisation.";
		return;
	}

	LOG(LogLevel::Info) << "Shuffling RGOs in provinces.";
	for (auto& prov : provinces)
	{
		bucketShuffler.putInBucket(prov.second);
	}
	bucketShuffler.shuffle();
}

void V2::V2World::importProvinces()
{
	std::set<std::string> provinceFilenames = discoverProvinceFilenames();
	for (auto provinceFilename : provinceFilenames)
	{
		auto newProvince = std::make_shared<Province>(provinceFilename, climateMapper, terrainDataMapper);
		provinces.insert(std::make_pair(newProvince->getID(), newProvince));
	}

	if (Utils::DoesFileExist("./blankMod/output/localisation/text.csv"))
	{
		importProvinceLocalizations("./blankMod/output/localisation/text.csv");
	}
	else
	{
		importProvinceLocalizations(theConfiguration.getVic2Path() + "/localisation/text.csv");
	}
	if (theConfiguration.getRandomiseRgos())
	{
		shuffleRgos();
	}
}

std::set<std::string> V2::V2World::discoverProvinceFilenames()
{
	std::set<std::string> provinceFilenames;
	if (Utils::doesFolderExist("./blankMod/output/history/provinces"))
	{
		Utils::GetAllFilesInFolderRecursive("./blankMod/output/history/provinces", provinceFilenames);
	}
	if (provinceFilenames.empty())
	{
		Utils::GetAllFilesInFolderRecursive(theConfiguration.getVic2Path() + "/history/provinces", provinceFilenames);
	}

	return provinceFilenames;
}

void V2::V2World::importProvinceLocalizations(const std::string& file)
{
	std::ifstream read(file);

	while (read.good() && !read.eof())
	{
		std::string line;
		getline(read, line);
		if (isAProvinceLocalization(line))
		{
			int position = line.find_first_of(';');
			int num = stoi(line.substr(4, position - 4));
			std::string name = line.substr(position + 1, line.find_first_of(';', position + 1) - position - 1);

			auto province = provinces.find(num);
			if (province != provinces.end())
			{
				province->second->setName(name);
			}
		}
	}
	read.close();
}

bool V2::V2World::isAProvinceLocalization(const std::string& line)
{
	return (line.substr(0, 4) == "PROV") && (isdigit(line[4]));
}

void V2::V2World::importDefaultPops()
{
	totalWorldPopulation = 0;

	std::set<std::string> filenames;
	Utils::GetAllFilesInFolder("./blankMod/output/history/pops/1836.1.1/", filenames);

	LOG(LogLevel::Info) << "Parsing minority pops mappings";

	for (auto filename : filenames)
	{
		importPopsFromFile(filename, minorityPopMapper);
	}


}

void V2::V2World::importPopsFromFile(const std::string& filename, const mappers::MinorityPopMapper& minorityPopMapper)
{
	std::list<int> popProvinces;

	std::ifstream popFile("./blankMod/output/history/pops/1836.1.1/" + filename);
	const mappers::PopMapper popMapper(popFile);
	popFile.close();

	for (const auto& provinceItr : popMapper.getProvincePopTypeMap())
	{
		int provinceNum = provinceItr.first;
		popProvinces.push_back(provinceNum);

		importPopsFromProvince(provinceNum, provinceItr.second, minorityPopMapper);
	}

	popRegions.insert(std::make_pair(filename, popProvinces));
}

void V2::V2World::importPopsFromProvince(const int provinceID, const mappers::PopTypes& popType, const mappers::MinorityPopMapper& minorityPopMapper)
{
	int provincePopulation = 0;
	int provinceSlavePopulation = 0;

	auto province = provinces.find(provinceID);
	if (province == provinces.end())
	{
		LOG(LogLevel::Warning) << "Could not find province " << provinceID << " for original pops.";
		return;
	}

	for (const auto& pop: popType.getPopTypes())
	{
		auto newPop = std::make_shared<V2::Pop>(pop.first, pop.second.getSize(), pop.second.getCulture(), pop.second.getReligion());
		province->second->addVanillaPop(newPop);
		if (minorityPopMapper.matchMinorityPop(newPop))
		{
			province->second->addMinorityPop(newPop);
		}

		totalWorldPopulation += newPop->getSize();
		provincePopulation += newPop->getSize();
		if (newPop->isSlavePop())
		{
			provinceSlavePopulation += newPop->getSize();
		}
	}

	province->second->setSlaveProportion(1.0 * provinceSlavePopulation / provincePopulation);
}

void V2::V2World::logPopsByCountry() const
{
	std::map<std::string, std::map<std::string, long int>> popsByCountry; // country, poptype, num

	std::set<std::string> filenames;
	Utils::GetAllFilesInFolder("./blankMod/output/history/pops/1836.1.1/", filenames);
	for (auto filename : filenames)
	{
		logPopsFromFile(filename, popsByCountry);
	}

	outputLog(popsByCountry);
}

void V2::V2World::logPopsFromFile(const std::string& filename, std::map<std::string, std::map<std::string, long int>>& popsByCountry) const
{
	std::ifstream popFile("./blankMod/output/history/pops/1836.1.1/" + filename);
	mappers::PopMapper popMapper(popFile);
	popFile.close();

	for (const auto& provinceItr : popMapper.getProvincePopTypeMap())
	{
		logPopsInProvince(provinceItr.first, provinceItr.second, popsByCountry);
	}
}

void V2::V2World::logPopsInProvince(const int& provinceID, const mappers::PopTypes& popTypes, std::map<std::string, std::map<std::string, long int>>& popsByCountry) const
{
	auto province = provinces.find(provinceID);
	if (province == provinces.end())
	{
		LOG(LogLevel::Warning) << "Could not find province " << provinceID << " for original pops.";
		return;
	}

	auto countryPopItr = getCountryForPopLogging(province->second->getOwner(), popsByCountry);

	for (const auto& popType : popTypes.getPopTypes())
	{
		logPop(popType.first, V2::Pop(popType.first, popType.second), countryPopItr);
	}
}

void V2::V2World::logPop(const std::string& popType, const V2::Pop& pop, std::map<std::string, std::map<std::string, long int>>::iterator countryPopItr) const
{
	auto popItr = countryPopItr->second.find(popType);
	if (popItr == countryPopItr->second.end())
	{
		long int newPopSize = 0;
		std::pair<std::map<std::string, long int>::iterator, bool> newIterator = countryPopItr->second.insert(make_pair(popType, newPopSize));
		popItr = newIterator.first;
	}
	popItr->second += pop.getSize();
}

std::map<std::string, std::map<std::string, long int>>::iterator V2::V2World::getCountryForPopLogging(std::string country, std::map<std::string, std::map<std::string, long int>>& popsByCountry) const
{
	auto countryPopItr = popsByCountry.find(country);
	if (countryPopItr == popsByCountry.end())
	{
		std::map<std::string, long int> newCountryPop;
		auto newIterator = popsByCountry.insert(make_pair(country, newCountryPop));
		countryPopItr = newIterator.first;
	}

	return countryPopItr;
}

void V2::V2World::outputLog(const std::map<std::string, std::map<std::string, long int>>& popsByCountry) const
{
	for (auto countryItr : popsByCountry)
	{
		long int total = 0;
		for (auto popsItr : countryItr.second)
		{
			total += popsItr.second;
		}

		for (auto popsItr : countryItr.second)
		{
			LOG(LogLevel::Info) << "," << countryItr.first << "," << popsItr.first << "," << popsItr.second << "," << static_cast<double>(popsItr.second / total);
		}

		LOG(LogLevel::Info) << "," << countryItr.first << "," << "Total," << total << "," << static_cast<double>(total / total);
	}
}

void V2::V2World::findCoastalProvinces()
{
	for (const auto& navalProvinceID : navalBaseMapper.getNavalProvinces())
	{
		auto province = provinces.find(navalProvinceID);
		if (province != provinces.end())
		{
			province->second->setCoastal();
		}
	}
}

void V2::V2World::importPotentialCountries()
{
	potentialCountries.clear();
	dynamicCountries.clear();

	std::ifstream V2CountriesInput;
	V2CountriesInput.open("./blankMod/output/common/countries.txt");
	if (!V2CountriesInput.is_open())
	{
		LOG(LogLevel::Error) << "Could not open countries.txt. The converter may be corrupted, try downloading it again.";
		exit(-1);
	}

	bool dynamicSection = false;
	while (!V2CountriesInput.eof())
	{
		std::string line;
		getline(V2CountriesInput, line);

		if ((line[0] == '#') || (line.size() < 3))
		{
			continue;
		}
		else if (line.substr(0, 12) == "dynamic_tags")
		{
			dynamicSection = true;
			continue;
		}

		importPotentialCountry(line, dynamicSection);
	}

	V2CountriesInput.close();
}

void V2::V2World::importPotentialCountry(const std::string& line, bool dynamicCountry)
{
	std::string tag = line.substr(0, 3);

	V2Country* newCountry = new V2Country(line, this, dynamicCountry);
	potentialCountries.insert(std::make_pair(tag, newCountry));
	if (dynamicCountry)
	{
		dynamicCountries.insert(std::make_pair(tag, newCountry));
	}
}

void V2::V2World::initializeCultureMappers(const EU4::World& sourceWorld)
{
	LOG(LogLevel::Info) << "Parsing culture mappings.";
	cultureMapper.loadFile("configurables/culture_map.txt");
	
	LOG(LogLevel::Info) << "Parsing slave culture mappings.";
	slaveCultureMapper.loadFile("configurables/culture_map_slaves.txt");
}

void V2::V2World::convertCountries(const EU4::World& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper)
{
	initializeCountries(sourceWorld, ideaEffectMapper);
	convertNationalValues(ideaEffectMapper);
	convertPrestige();
	addAllPotentialCountries();
}

void V2::V2World::initializeCountries(const EU4::World& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper)
{
	for (auto sourceCountry: sourceWorld.getCountries())
	{
		const std::string& V2Tag = countryMapper.getV2Tag(sourceCountry.first);
		if (V2Tag == "")
		{
			LOG(LogLevel::Error) << "EU4 tag " << sourceCountry.first << " is unmapped and cannot be converted.";
			exit(-1);
		}

		V2Country* destCountry = createOrLocateCountry(V2Tag, sourceCountry.second);
		destCountry->initFromEU4Country(
			sourceWorld.getRegions(),
			sourceCountry.second,
			techSchoolMapper,
			cultureMapper,
			slaveCultureMapper,
			ideaEffectMapper,
			religionMapper,
			provinceMapper,
			governmentMapper,
			countryMapper
		);
		countries.insert(std::make_pair(V2Tag, destCountry));
	}
}

V2::V2Country* V2::V2World::createOrLocateCountry(const std::string& V2Tag, const std::shared_ptr<EU4::Country> sourceCountry)
{
	V2Country* destCountry = nullptr;

	auto potentialCountry = potentialCountries.find(V2Tag);
	if (potentialCountry == potentialCountries.end())
	{
		std::string countryFileName = sourceCountry->getName() + ".txt";
		destCountry = new V2Country(V2Tag, countryFileName, this);
	}
	else
	{
		destCountry = potentialCountry->second;
	}

	return destCountry;
}

bool scoresSorter(std::pair<V2::V2Country*, double> first, std::pair<V2::V2Country*, double> second)
{
	return (first.second > second.second);
}

void V2::V2World::convertNationalValues(const mappers::IdeaEffectMapper& ideaEffectMapper)
{
	// set national values
	std::list<std::pair<V2Country*, double>> libertyScores;
	std::list<std::pair<V2Country*, double>> equalityScores;
	std::set<V2Country*>					valuesUnset;
	for (std::map<std::string, V2Country*>::iterator countryItr = countries.begin(); countryItr != countries.end(); ++countryItr)
	{
		double libertyScore;
		double equalityScore;
		double orderScore;
		std::tie(libertyScore, equalityScore, orderScore) = countryItr->second->getNationalValueScores();

		if (libertyScore > orderScore)
		{
			libertyScores.push_back(std::make_pair(countryItr->second, libertyScore));
		}
		if ((equalityScore > orderScore) && (equalityScore > libertyScore))
		{
			equalityScores.push_back(std::make_pair(countryItr->second, equalityScore));
		}
		valuesUnset.insert(countryItr->second);
	}
	equalityScores.sort(scoresSorter);
	int equalityLeft = 5;
	for (std::list<std::pair<V2Country*, double> >::iterator equalItr = equalityScores.begin(); equalItr != equalityScores.end(); ++equalItr)
	{
		if (equalityLeft < 1)
		{
			break;
		}
		std::set<V2Country*>::iterator unsetItr = valuesUnset.find(equalItr->first);
		if (unsetItr != valuesUnset.end())
		{
			valuesUnset.erase(unsetItr);
			equalItr->first->setNationalValue("nv_equality");
			equalityLeft--;
		}
	}
	libertyScores.sort(scoresSorter);
	int libertyLeft = 20;
	for (std::list<std::pair<V2Country*, double> >::iterator libItr = libertyScores.begin(); libItr != libertyScores.end(); ++libItr)
	{
		if (libertyLeft < 1)
		{
			break;
		}
		std::set<V2Country*>::iterator unsetItr = valuesUnset.find(libItr->first);
		if (unsetItr != valuesUnset.end())
		{
			valuesUnset.erase(unsetItr);
			libItr->first->setNationalValue("nv_liberty");
			libertyLeft--;
		}
	}
	for (std::set<V2Country*>::iterator unsetItr = valuesUnset.begin(); unsetItr != valuesUnset.end(); ++unsetItr)
	{
		(*unsetItr)->setNationalValue("nv_order");
	}
}

void V2::V2World::convertPrestige()
{
	LOG(LogLevel::Info) << "-> Setting prestige";

	double highestScore = 0.0;
	for (auto country: countries)
	{
		double score = 0.0;
		auto srcCountry = country.second->getSourceCountry();
		if (srcCountry != nullptr)
		{
			score = srcCountry->getScore();
		}
		if (score > highestScore)
		{
			highestScore = score;
		}
	}

	for (auto country: countries)
	{
		double score = 0.0;
		auto srcCountry = country.second->getSourceCountry();
		if (srcCountry != nullptr)
		{
			score = srcCountry->getScore();
		}
		double prestige = 0.0;
		if (highestScore > 0)
		{
			prestige = score / highestScore * 100.0;
		}
		country.second->addPrestige(prestige);
	}
}

void V2::V2World::addAllPotentialCountries()
{
	// ALL potential countries should be output to the file, otherwise some things don't get initialized right when loading Vic2
	for (auto potentialCountry : potentialCountries)
	{
		std::map<std::string, V2Country*>::iterator citr = countries.find(potentialCountry.first);
		if (citr == countries.end())
		{
			potentialCountry.second->initFromHistory(unreleasablesMapper);
			countries.insert(make_pair(potentialCountry.first, potentialCountry.second));
		}
	}
}

unsigned int V2::V2World::countCivilizedNations()
{
	unsigned int numPotentialGPs = 0;
	for (auto country : countries)
	{
		auto states = country.second->getStates();
		if ((country.second->isCivilized())&&(states.size() > 1))
		{
			numPotentialGPs++;
		}
	}

	return numPotentialGPs;
}

struct MTo1ProvinceComp
{
	std::vector<const EU4::Province*> provinces;
};

void V2::V2World::convertProvinces(const EU4::World& sourceWorld)
{
	for (auto Vic2Province : provinces)
	{
		auto EU4ProvinceNumbers = provinceMapper.getEU4ProvinceNumbers(Vic2Province.first);
		if (EU4ProvinceNumbers.size() == 0)
		{
			LOG(LogLevel::Warning) << "No source for " << Vic2Province.second->getName() << " (province " << Vic2Province.first << ')';
			continue;
		}
		else if (*EU4ProvinceNumbers.begin() == 0)
		{
			continue;
		}
		else if (
			(theConfiguration.getResetProvinces() == "yes") &&
			provinceMapper.isProvinceResettable(Vic2Province.first, "resettableRegion")
		) {
			Vic2Province.second->setResettable();
			continue;
		}

		Vic2Province.second->clearCores();

		const EU4::Province* oldProvince = nullptr;
		std::string oldOwnerTag;
		std::string oldControllerTag;
		// determine ownership and controllership by province count
		std::map<std::string, MTo1ProvinceComp> provinceOwnerBins;
		std::map<std::string, MTo1ProvinceComp> provinceControllerBins;
		double newProvinceTotalBaseTax = 0;
		for (auto EU4ProvinceNumber : EU4ProvinceNumbers)
		{
			const EU4::Province& province = sourceWorld.getProvince(EU4ProvinceNumber);
			auto ownerTag = province.getOwnerString();
			auto controllerTag = province.getControllerString();
			if (provinceOwnerBins.find(ownerTag) == provinceOwnerBins.end())
			{
				provinceOwnerBins[ownerTag] = MTo1ProvinceComp();
			}
			if (provinceControllerBins.find(controllerTag) == provinceControllerBins.end())
			{
				provinceControllerBins[controllerTag] = MTo1ProvinceComp();
			}
			provinceOwnerBins[ownerTag].provinces.push_back(&province);
			provinceControllerBins[controllerTag].provinces.push_back(&province);
			newProvinceTotalBaseTax += province.getBaseTax();
			// I am the new owner if there is no current owner, or I have more provinces than the current owner,
			// or I have the same number of provinces than the current owner
			if (
				(oldOwnerTag == "") ||
				(provinceOwnerBins[ownerTag].provinces.size() > provinceOwnerBins[oldOwnerTag].provinces.size()) ||
				(provinceOwnerBins[ownerTag].provinces.size() == provinceOwnerBins[oldOwnerTag].provinces.size())
				)
			{
				oldOwnerTag = ownerTag;
				oldProvince = &province;
			}
			// I am the new controller if there is no current controller, or I have more provinces than the current controller,
			// or I have the same number of provinces than the current controller
			if	(
					(oldControllerTag == "") ||
					(
						provinceControllerBins[controllerTag].provinces.size() >
						provinceControllerBins[oldControllerTag].provinces.size()
					) ||
					(
						provinceControllerBins[controllerTag].provinces.size() ==
						provinceControllerBins[oldControllerTag].provinces.size()
					)
				)
			{
				oldControllerTag = controllerTag;
			}
		}
		if (oldOwnerTag == "")
		{
			Vic2Province.second->setOwner("");
			Vic2Province.second->setController("");
			continue;
		}

		const std::string& V2ControllerTag = countryMapper.getV2Tag(oldControllerTag);
		const std::string& V2OwnerTag = countryMapper.getV2Tag(oldOwnerTag);
		if (V2OwnerTag.empty())
		{
			LOG(LogLevel::Warning) << "Could not map provinces owned by " << oldOwnerTag;
		}
		else if (V2ControllerTag.empty())
		{
			LOG(LogLevel::Warning) << "Could not map provinces controlled by " << V2ControllerTag;
		}
		else
		{
			Vic2Province.second->setOwner(V2OwnerTag);
			Vic2Province.second->setController(V2ControllerTag);
			std::map<std::string, V2Country*>::iterator ownerItr = countries.find(V2OwnerTag);
			if (ownerItr != countries.end())
			{
				ownerItr->second->addProvince(Vic2Province.second);
			}
			Vic2Province.second->convertFromOldProvince(
				oldProvince,
				sourceWorld.getCountries()
			);

			for (auto provinceBin: provinceOwnerBins)
			{
				for (auto sourceProvince: provinceBin.second.provinces)
				{
					// assign cores
					auto oldCores = sourceProvince->getCores();
					for (auto oldCore: oldCores)
					{
						// skip this core if the country is the owner of the EU4 province but not the V2 province
						// (i.e. "avoid boundary conflicts that didn't exist in EU4").
						// this country may still get core via a province that DID belong to the current V2 owner
						if ((oldCore == provinceBin.first) && (oldCore != oldOwnerTag))
						{
							continue;
						}

						const std::string& coreV2Tag = countryMapper.getV2Tag(oldCore);
						if (!coreV2Tag.empty())
						{
							Vic2Province.second->addCore(coreV2Tag);
						}
					}

					// determine demographics
					double provPopRatio = sourceProvince->getBaseTax() / newProvinceTotalBaseTax;
					auto popRatios = sourceProvince->getPopRatios();
					std::vector<V2::Demographic> demographics = determineDemographics(
						sourceWorld.getRegions(),
						popRatios,
						sourceProvince,
						Vic2Province.second,
						oldOwnerTag,
						Vic2Province.first,
						provPopRatio
					);
					for (auto demographic: demographics)
					{
						Vic2Province.second->addPopDemographic(demographic);
					}

					// set forts and naval bases
					if (
						sourceProvince->hasBuilding("fort4") ||
						sourceProvince->hasBuilding("fort5") ||
						sourceProvince->hasBuilding("fort6")
					) {
						Vic2Province.second->setFortLevel(1);
					}
				}
			}
		}
	}
}

std::vector<V2::Demographic> V2::V2World::determineDemographics(
	const EU4::Regions& eu4Regions,
	std::vector<EU4::PopRatio>& popRatios,
	const EU4::Province* eProv,
	std::shared_ptr<V2::Province> vProv,
	std::string oldOwnerTag,
	int destNum,
	double provPopRatio
)
{
	std::vector<V2::Demographic> demographics;
	for (auto popRatio: popRatios)
	{
		std::optional<std::string> dstCulture;
		dstCulture = cultureMapper.cultureMatch(
			eu4Regions,
			popRatio.getCulture(),
			popRatio.getReligion(),
			eProv->getNum(),
			oldOwnerTag
		);
		if (!dstCulture)
		{
			LOG(LogLevel::Warning) << "Could not set culture for pops in Vic2 province " << destNum;
			dstCulture = "no_culture";
		}

		std::optional<std::string> religion = religionMapper.getVic2Religion(popRatio.getReligion());
		if (!religion)
		{
			LOG(LogLevel::Warning) << "Could not set religion for pops in Vic2 province " << destNum;
			religion = "";
		}

		std::optional<std::string> slaveCulture;
		slaveCulture = slaveCultureMapper.cultureMatch(
			eu4Regions,
			popRatio.getCulture(),
			popRatio.getReligion(),
			eProv->getNum(),
			oldOwnerTag
		);
		if (!slaveCulture)
		{
			auto thisContinent = continentsMapper.getEU4Continent(eProv->getNum());
			if ((thisContinent) && ((thisContinent == "asia") || (thisContinent == "oceania")))
			{
				if (theConfiguration.getDebug())
				{
					LOG(LogLevel::Warning) << "No mapping for slave culture in province "
						<< destNum << " - using native culture (" << popRatio.getCulture() << ").";
				}
				slaveCulture = popRatio.getCulture();
			}
			else
			{
				if (theConfiguration.getDebug())
				{
					LOG(LogLevel::Warning) << "No mapping for slave culture for pops in Vic2 province "
						<< destNum << " - using african_minor.";
				}
				slaveCulture = "african_minor";
			}
		}

		V2::Demographic demographic;
		demographic.culture = *dstCulture;
		demographic.slaveCulture = *slaveCulture;
		demographic.religion = *religion;
		demographic.upperRatio = popRatio.getUpperRatio() * provPopRatio;
		demographic.middleRatio = popRatio.getMiddleRatio() * provPopRatio;
		demographic.lowerRatio = popRatio.getLowerRatio() * provPopRatio;

		if (theConfiguration.getDebug())
		{
			LOG(LogLevel::Info) << "EU4 Province " << eProv->getNum() << ", "
				<< "Vic2 Province " << vProv->getID() << ", "
				<< "Culture: " << demographic.culture << ", "
				<< "Religion: " << demographic.religion << ", "
				<< "upperPopRatio: " << popRatio.getUpperRatio() << ", "
				<< "middlePopRatio: " << popRatio.getMiddleRatio() << ", "
				<< "lowerPopRatio: " << popRatio.getLowerRatio() << ", "
				<< "provPopRatio: " << provPopRatio << ", "
				<< "upperRatio: " << demographic.upperRatio << ", "
				<< "middleRatio: " << demographic.middleRatio << ", "
				<< "lowerRatio: " << demographic.lowerRatio;
		}
		demographics.push_back(demographic);
	}

	return demographics;
}

void V2::V2World::setupColonies()
{
	for (auto& countryItr : countries)
	{
		// find all land connections to capitals
		auto openProvinces = provinces;
		std::queue<int> goodProvinces;

		auto openItr = openProvinces.find(countryItr.second->getCapital());
		if (openItr == openProvinces.end()) continue;

		// if the capital is not owned, don't bother running
		if (openItr->second->getOwner() != countryItr.first) continue;

		openItr->second->setLandConnection(true);
		goodProvinces.push(openItr->first);
		openProvinces.erase(openItr);

		do
		{
			int currentProvince = goodProvinces.front();
			goodProvinces.pop();
			auto adjacencies = adjacencyMapper.getVic2Adjacencies(currentProvince);
			if (adjacencies)
			{
				for (auto adjacency: *adjacencies)
				{
					auto openItr2 = openProvinces.find(adjacency);
					if (openItr2 == openProvinces.end()) continue;
					if (openItr2->second->getOwner() != countryItr.first) continue;
					openItr2->second->setLandConnection(true);
					goodProvinces.push(openItr2->first);
					openProvinces.erase(openItr2);
				}
			}
		} while (!goodProvinces.empty());

		// find all provinces on the same continent as the owner's capital
		std::optional<std::string> capitalContinent;
		auto capital = provinces.find(countryItr.second->getCapital());
		if (capital != provinces.end())
		{
			if (capital->second->getEU4ID() > 0)
			{
				capitalContinent = continentsMapper.getEU4Continent(capital->second->getEU4ID());
				if (!capitalContinent) continue;
			}
		}
		else
		{
			continue;
		}
		auto ownedProvinces = countryItr.second->getProvinces();
		for (auto& ownedProvince: ownedProvinces)
		{
			std::optional<std::string> continent = continentsMapper.getEU4Continent(ownedProvince.second->getEU4ID());			
			if (continent && continent == capitalContinent)
			{
				ownedProvince.second->setSameContinent();
			}
		}
	}

	for (auto& provItr : provinces)
	{
		provItr.second->determineColonial();
	}
}

void V2::V2World::setupStates()
{
	std::list<std::shared_ptr<V2::Province>> unassignedProvs;
	for (std::map<int, std::shared_ptr<V2::Province>>::iterator itr = provinces.begin(); itr != provinces.end(); ++itr)
	{
		unassignedProvs.push_back(itr->second);
	}

	std::list<std::shared_ptr<V2::Province>>::iterator iter;
	while (unassignedProvs.size() > 0)
	{
		iter = unassignedProvs.begin();
		int provId = (*iter)->getID();
		std::string owner = (*iter)->getOwner();

		if (owner == "")
		{
			unassignedProvs.erase(iter);
			continue;
		}

		State* newState = new State(stateId, *iter);
		stateId++;
		auto neighbors = stateMapper.getAllProvincesInState(provId);

		bool colonial = (*iter)->isColony();
		newState->setColonial(colonial);
		iter = unassignedProvs.erase(iter);

		for (auto i = neighbors.begin(); i != neighbors.end(); i++)
		{
			for (iter = unassignedProvs.begin(); iter != unassignedProvs.end(); iter++)
			{
				if ((*iter)->getID() == *i)
				{
					if ((*iter)->getOwner() == owner)
					{
						if ((*iter)->isColony() == colonial)
						{
							newState->addProvince(*iter);
							iter = unassignedProvs.erase(iter);
						}
					}
				}
			}
		}
		newState->colloectNavalBase();
		std::map<std::string, V2Country*>::iterator iter2 = countries.find(owner);
		if (iter2 != countries.end())
		{
			iter2->second->addState(newState, portProvincesMapper);
		}
	}
}

void V2::V2World::convertUncivReforms(const EU4::World& sourceWorld, const mappers::TechGroupsMapper& techGroupsMapper)
{
	// tech group

	enum civConversion { older, newer };
	civConversion techGroupAlgoritm = newer;
	double topTech = 96;
	int topInstitutions = 7;
	auto version18 = EU4::Version("1.18.0");
	if (sourceWorld.getVersion() >= version18)
	{
		techGroupAlgorithm  = newer;

		// Find global max tech and institutions embraced

		auto sourceCountries = sourceWorld.getCountries();


		auto i = sourceCountries.begin();
		while (i->second->getProvinces().size() == 0)
			i++;

		// Take max from the first country
		auto currCountry = i->second;

		double totalTechs = currCountry->getMilTech() + currCountry->getAdmTech() + currCountry->getDipTech();
		topTech = totalTechs;
		int currInstitutions = currCountry->numEmbracedInstitutions();
		topInstitutions = currInstitutions;
		
		int num = 2;

		// Calculate max
		for (i++; i != sourceCountries.end(); i++)
		{
			currCountry = i->second;
			if (currCountry->getProvinces().size() == 0)
				continue;
			
			totalTechs = currCountry->getMilTech() + currCountry->getAdmTech() + currCountry->getDipTech();
			if (totalTechs > topTech)
				topTech = totalTechs;

			currInstitutions = currCountry->numEmbracedInstitutions();
			if (currInstitutions > topInstitutions)
				topInstitutions = currInstitutions;

			num++;
		}
	}
	else
	{
		techGroupAlgorithm = older;
	}

	for (std::map<std::string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
	{
		itr->second->convertUncivReforms(techGroupAlgorithm, topTech, topInstitutions, techGroupsMapper);
	}

	// inherit civilisation level for landless countries from their capital's owner
	for (std::map<std::string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
	{
		if (itr->second->getProvinces().size() == 0)
		{
			int capitalNum = itr->second->getCapital();
			if (capitalNum == 0)
				continue;
			std::shared_ptr<V2::Province> capital = getProvince(capitalNum);
			std::string capOwnerTag = capital->getOwner();
			V2Country* capOwner = getCountry(capOwnerTag);
			if (capOwner == nullptr)
				continue;
			itr->second->convertLandlessReforms(capOwner);
		}

	}
}


void V2::V2World::convertTechs(const EU4::World& sourceWorld)
{
	helpers::TechValues techValues(countries);

	for (auto countryItr: countries)
	{
		auto country = countryItr.second;
		if (techValues.isValidCountryForTechConversion(country))
		{
			country->setArmyTech(techValues.getNormalizedArmyTech(*country->getSourceCountry()));
			country->setNavyTech(techValues.getNormalizedNavyTech(*country->getSourceCountry()));
			country->setCommerceTech(techValues.getNormalizedCommerceTech(*country->getSourceCountry()));
			country->setCultureTech(techValues.getNormalizedCultureTech(*country->getSourceCountry()));
			country->setIndustryTech(techValues.getNormalizedIndustryTech(*country->getSourceCountry()));
		}
	}
}


void V2::V2World::allocateFactories(const EU4::World& sourceWorld)
{
	// determine average production tech
	auto sourceCountries = sourceWorld.getCountries();
	double admMean = 0.0f;
	int num = 1;
	for (auto itr = sourceCountries.begin(); itr != sourceCountries.end(); ++itr)
	{
		if ((itr)->second->getProvinces().size() == 0)
		{
			continue;
		}

		double admTech = (itr)->second->getAdmTech();
		admMean += ((admTech - admMean) / num);
		++num;
	}

	// give all extant civilized nations an industrial score
	std::deque<std::pair<double, V2Country*>> weightedCountries;
	for (std::map<std::string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
	{
		if (!itr->second->isCivilized())
		{
			continue;
		}

		auto sourceCountry = itr->second->getSourceCountry();
		if (sourceCountry == nullptr)
		{
			continue;
		}

		if (itr->second->getProvinces().size() == 0)
		{
			continue;
		}

		// modified manufactory weight follows diminishing returns curve y = x^(3/4)+log((x^2)/5+1)
		int manuCount = sourceCountry->getManufactoryCount();
		double manuWeight = pow(manuCount, 0.75) + log1p(static_cast<double>(pow(manuCount, 2)) / 5.0);
		double industryWeight = (sourceCountry->getAdmTech() - admMean) + manuWeight;
		// having one manufactory and average tech is not enough; you must have more than one, or above-average tech
		if (industryWeight > 1.0)
		{
			weightedCountries.push_back(std::pair<double, V2Country*>(industryWeight, itr->second));
		}
	}
	if (weightedCountries.size() < 1)
	{
		LOG(LogLevel::Warning) << "No countries are able to accept factories";
		return;
	}
	sort(weightedCountries.begin(), weightedCountries.end());

	// allow a maximum of 10 (plus any tied at tenth place) countries to recieve factories
	std::deque<std::pair<double, V2Country*>> restrictCountries;
	double threshold = 1.0;
	double totalIndWeight = 0.0;
	for (std::deque<std::pair<double, V2Country*>>::reverse_iterator itr = weightedCountries.rbegin(); itr != weightedCountries.rend(); ++itr)
	{
		if ((restrictCountries.size() > 10) && (itr->first < (threshold - FLT_EPSILON)))
		{
			break;
		}
		restrictCountries.push_front(*itr); // preserve sort
		totalIndWeight += itr->first;
		threshold = itr->first;
	}

	if (totalIndWeight == 0)
	{
		LOG(LogLevel::Warning) << "The world is a backwater! No factories for anyone!";
		return;
	} 

	weightedCountries.swap(restrictCountries);
	// remove nations that won't have enough industiral score for even one factory
	std::deque<std::shared_ptr<Factory>> factoryList = factoryTypeMapper.buildFactories();
	while (((weightedCountries.begin()->first / totalIndWeight) * factoryList.size() + 0.5 /*round*/) < 1.0)
	{
		weightedCountries.pop_front();
		if (weightedCountries.size() == 0)
		{
			LOG(LogLevel::Warning) << "These are all primitives! No factories for anyone!";
			return;
		}
	}

	// determine how many factories each eligible nation gets
	std::vector<std::pair<int, V2Country*>> factoryCounts;
	for (std::deque<std::pair<double, V2Country*>>::iterator itr = weightedCountries.begin(); itr != weightedCountries.end(); ++itr)
	{
		int factories = int(((itr->first / totalIndWeight) * factoryList.size()) + 0.5 /*round*/);
		factoryCounts.push_back(std::pair<int, V2Country*>(factories, itr->second));
	}

	// allocate the factories
	std::vector<std::pair<int, V2Country*>>::iterator lastReceptiveCountry = factoryCounts.end()--;
	std::vector<std::pair<int, V2Country*>>::iterator citr = factoryCounts.begin();
	while (!factoryList.empty())
	{
		bool accepted = false;
		if (citr->first > 0) // can take more factories
		{
			for (std::deque<std::shared_ptr<Factory>>::iterator qitr = factoryList.begin(); qitr != factoryList.end(); ++qitr)
			{
				if (citr->second->addFactory(*qitr))
				{
					--(citr->first);
					lastReceptiveCountry = citr;
					accepted = true;
					factoryList.erase(qitr);
					break;
				}
			}
		}
		if (!accepted && citr == lastReceptiveCountry)
		{
			break;
		}
		if (++citr == factoryCounts.end())
		{
			citr = factoryCounts.begin(); // loop around to beginning
		}
	}
}

void V2::V2World::setupPops(const EU4::World& sourceWorld)
{
	long		my_totalWorldPopulation = static_cast<long>(0.55 * totalWorldPopulation);
	double	popWeightRatio = my_totalWorldPopulation / sourceWorld.getTotalProvinceWeights();

	//ofstream output_file("Data.csv");

	int popAlgorithm;
	auto version12 = EU4::Version("1.12.0");
	if (sourceWorld.getVersion() >= version12)
	{
		popAlgorithm = 2;
	}
	else
	{
		popAlgorithm = 1;
	}

	for (std::map<std::string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
	{
		itr->second->setupPops(popWeightRatio, popAlgorithm, provinceMapper);
	}

	LOG(LogLevel::Info) << "Vanilla world population: " << totalWorldPopulation;
	if (theConfiguration.getPopShaping() == Configuration::POPSHAPES::Extreme)
	{
		LOG(LogLevel::Info) << "\tModified world population: " << my_totalWorldPopulation;
		LOG(LogLevel::Info) << "\tTotal world weight sum: " << sourceWorld.getTotalProvinceWeights();
		LOG(LogLevel::Info) << "\t" << my_totalWorldPopulation << " / " << sourceWorld.getTotalProvinceWeights();
		LOG(LogLevel::Info) << "\tPopulation per weight point is: " << popWeightRatio;
	}
	long newTotalPopulation = 0;
	for (auto province: provinces) newTotalPopulation += province.second->getTotalPopulation();
	LOG(LogLevel::Info) << "New total world population: " << newTotalPopulation;

	// Heading
	/*output_file << "EU ID"		<< ",";
	output_file << "EU NAME"	<< ",";
	output_file << "OWNER"		<< ",";
	output_file << "BTAX"		<< ",";
	output_file << "TX INCOME"	<< ",";
	output_file << "PROD"		<< ",";
	output_file << "MP"			<< ",";
	output_file << "BUIDINGS"	<< ",";
	output_file << "TRADE"		<< ",";
	output_file << "TOTAL"		<< ",";
	output_file << "#DEST"		<< ",";
	output_file << "V2 ID"		<< ",";
	output_file << "V2 NAME"	<< ",";
	output_file << "CALC POPS"	<< ",";
	output_file << "POPS"		<< endl;
	for (auto itr = provinces.begin(); itr != provinces.end(); itr++)
	{
		// EU4ID, EU4Name, EU4TAG, BTX, TAX, PROD, MP, BUILD, TRADE, WEIGHT, DESTV2, V2Name, POPs //
		newTotalPopulation += itr->second->getTotalPopulation();

		//	EU4 Province ID
		//if (itr->second->getSrcProvince() != nullptr)
		//{
		//	output_file << itr->second->getSrcProvince()->getNum() << ",";
		//}
		//else
		//{
		//	continue;
		//}
		////	EU4 Province Name
		//if (itr->second->getSrcProvince() != nullptr)
		//{
		//	output_file << itr->second->getSrcProvince()->getName() << ",";
		//}
		//else
		//{
		//	output_file << "SEA" << ",";
		//}
		////	EU4 Province Owner
		//if (itr->second->getSrcProvince() != nullptr)
		//{
		//	output_file << itr->second->getSrcProvince()->getOwnerString() << ",";
		//}
		//else
		//{
		//	output_file << "nullptr" << ",";
		//}
		////	EU4 Base Tax
		//if (itr->second->getSrcProvince() != nullptr)
		//{
		//	output_file << (2 * itr->second->getSrcProvince()->getBaseTax()) << ",";
		//}
		//else
		//{
		//	output_file << -1 << ",";
		//}
		////	EU4 Total Tax Income
		//if (itr->second->getSrcProvince() != nullptr)
		//{
		//	output_file << 2*(itr->second->getSrcProvince()->getProvTaxIncome()) << ",";
		//}
		//else
		//{
		//	output_file << -1 << ",";
		//}
		////	EU4 Total Prod Income
		//if (itr->second->getSrcProvince() != nullptr)
		//{
		//	output_file << itr->second->getSrcProvince()->getProvProdIncome() << ",";
		//}
		//else
		//{
		//	output_file << -1 << ",";
		//}
		////	EU4 Total Manpower weight
		//if (itr->second->getSrcProvince() != nullptr)
		//{
		//	output_file << itr->second->getSrcProvince()->getProvMPWeight() << ",";
		//}
		//else
		//{
		//	output_file << -1 << ",";
		//}
		////	EU4 Total Building weight
		//if (itr->second->getSrcProvince() != nullptr)
		//{
		//	output_file << itr->second->getSrcProvince()->getProvTotalBuildingWeight() << ",";
		//}
		//else
		//{
		//	output_file << -1 << ",";
		//}
		////	EU4 Total Tradegoods weight
		//if (itr->second->getSrcProvince() != nullptr)
		//{
		//	output_file << itr->second->getSrcProvince()->getCurrTradeGoodWeight() << ",";
		//}
		//else
		//{
		//	output_file << -1 << ",";
		//}
		////	EU4 Province Weight
		//if (itr->second->getSrcProvince() != nullptr)
		//{
		//	output_file << itr->second->getSrcProvince()->getTotalWeight() << ",";
		//}
		//else
		//{
		//	output_file << -1 << ",";
		//}
		////	Number of DestV2Provs
		//auto Vic2Provinces = theProvinceMapper->getVic2ProvinceNumbers(itr->second->getSrcProvince()->getNum());
		//if (itr->second->getSrcProvince() != nullptr)
		//{
		//	output_file << Vic2Provinces.size() << ",";
		//}
		//else
		//{
		//	output_file << -2 << ",";
		//}
		////	V2 Province ID
		//output_file << itr->second->getNum() << ",";
		////	V2 Province Name
		//if (itr->second->getName() == "")
		//{
		//	output_file << itr->second->getNum() << ",";
		//}
		//else
		//{
		//	output_file << itr->second->getName() << ",";
		//}
		////	Calculated V2 POPs
		//output_file << ((itr->second->getSrcProvince()->getTotalWeight()*popWeightRatio)/Vic2Provinces.size()) << ",";
		////	V2 POPs
		//output_file << itr->second->getTotalPopulation() << endl;
	}
	LOG(LogLevel::Info) << "New total world population: " << newTotalPopulation;

	//output_file.close();*/
}


void V2::V2World::addUnions()
{
	if (theConfiguration.getCoreHandling() == Configuration::COREHANDLES::DropAll) return;

	LOG(LogLevel::Info) << "Parsing cultural union mappings.";
	culturalUnionMapper.loadFile("configurables/unions.txt");
	LOG(LogLevel::Info) << "Parsing nationalities mappings.";
	culturalNationalitiesMapper.loadFile("configurables/nationals.txt");

	LOG(LogLevel::Info) << "Distributing national and cultural union cores.";

	for (std::map<int, std::shared_ptr<V2::Province>>::iterator provItr = provinces.begin(); provItr != provinces.end(); ++provItr)
	{
		if (!provItr->second->wasColony())
		{
			auto cultures = provItr->second->getCulturesOverThreshold(0.5);
			for (auto culture : cultures)
			{
				std::vector<std::string> unionCores = culturalUnionMapper.getCoresForCulture(culture);
				std::vector<std::string> nationalCores = culturalNationalitiesMapper.getCoresForCulture(culture);
				switch (theConfiguration.getCoreHandling())
				{
				case Configuration::COREHANDLES::DropNational:
					for (auto core : unionCores)
					{
						provItr->second->addCore(core);
					}
					break;
				case Configuration::COREHANDLES::DropUnions:
					for (auto core : nationalCores)
					{
						LOG(LogLevel::Debug) << provItr->second->getName() << ": " << core;
						provItr->second->addCore(core);
					}
					break;
				case Configuration::COREHANDLES::DropNone:
					for (auto core : unionCores)
					{
						provItr->second->addCore(core);
					}
					for (auto core : nationalCores)
					{
						provItr->second->addCore(core);
					}
				case Configuration::COREHANDLES::DropAll:
				default:
					break;
				}
			}
		}
	}
}


//#define TEST_V2_PROVINCES
void V2::V2World::convertArmies(const EU4::World& sourceWorld)
{
	// convert leaders and armies

	for (std::map<std::string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
	{
		itr->second->convertLeaders(leaderTraitMapper);
		itr->second->convertArmies(regimentCostsMapper, provinces, portProvincesMapper, provinceMapper, adjacencyMapper);
	}
}

void V2::V2World::output(unsigned int potentialGPs) const
{
	LOG(LogLevel::Info) << "<- Copying Mod Template";
	Utils::copyFolder("blankMod/output", "output/output");
	LOG(LogLevel::Info) << "<- Moving Mod Template";
	Utils::renameFolder("output/output", "output/" + theConfiguration.getOutputName());
	LOG(LogLevel::Info) << "<- Crafting .mod File";
	createModFile();

	// Record converter version

	LOG(LogLevel::Info) << "<- Writing version";
	std::ofstream versionFile;

	try
	{
		versionFile.open("output/" + theConfiguration.getOutputName() + "/eu4tov2_version.txt");
		versionFile << "# 1.0K-prerelease \"Kurland\", built on " << __TIMESTAMP__ << ".\n";
		versionFile.close();
	}
	catch (const std::exception&)
	{
		LOG(LogLevel::Error) << "Error writing version file! Is the output folder writeable?";
	}

	// Update bookmark starting dates
	LOG(LogLevel::Info) << "<- Updating bookmarks";
	std::string startDate = "<STARTDATE>";
	std::string numGPs = "GREAT_NATIONS_COUNT = 8";

	std::ostringstream incomingDefines, incomingBookmarks;

	std::ifstream defines_lua("output/" + theConfiguration.getOutputName() + "/common/defines.lua");
	incomingDefines << defines_lua.rdbuf();
	defines_lua.close();
	std::string strDefines = incomingDefines.str();
	size_t pos1 = strDefines.find(startDate);
	strDefines.replace(pos1, startDate.length(), theConfiguration.getLastEU4Date().toString());

	if (potentialGPs < 8)
	{
		size_t posGPs = strDefines.find(numGPs);
		std::string replacementGPs = "GREAT_NATIONS_COUNT = " + std::to_string(potentialGPs);
		strDefines.replace(posGPs, numGPs.length(), replacementGPs);

	}

	std::ofstream out_defines_lua("output/" + theConfiguration.getOutputName() + "/common/defines.lua");
	out_defines_lua << strDefines;
	out_defines_lua.close();

	std::ifstream bookmarks_txt("output/" + theConfiguration.getOutputName() + "/common/bookmarks.txt");
	incomingBookmarks << bookmarks_txt.rdbuf();
	bookmarks_txt.close();
	std::string strBookmarks = incomingBookmarks.str();
	size_t pos2 = strBookmarks.find(startDate);
	strBookmarks.replace(pos2, startDate.length(), theConfiguration.getLastEU4Date().toString());
	std::ofstream out_bookmarks_txt("output/" + theConfiguration.getOutputName() + "/common/bookmarks.txt");
	out_bookmarks_txt << strBookmarks;
	out_bookmarks_txt.close();

	// Create common\countries path.
	std::string countriesPath = "output/" + theConfiguration.getOutputName() + "/common/countries";
	if (!Utils::TryCreateFolder(countriesPath))
	{
		return;
	}

	// Output common\countries.txt
	LOG(LogLevel::Info) << "<- Creating countries.txt";
	FILE* allCountriesFile;
	if (fopen_s(&allCountriesFile, ("output/" + theConfiguration.getOutputName() + "/common/countries.txt").c_str(), "w") != 0)
	{
		LOG(LogLevel::Error) << "Could not create countries file";
		exit(-1);
	}
	for (std::map<std::string, V2Country*>::const_iterator i = countries.begin(); i != countries.end(); ++i)
	{
		const V2Country& country = *i->second;
		std::map<std::string, V2Country*>::const_iterator j = dynamicCountries.find(country.getTag());
		if (j == dynamicCountries.end())
		{
			country.outputToCommonCountriesFile(allCountriesFile);
		}
	}
	fprintf(allCountriesFile, "\n");
	fprintf(allCountriesFile, "##HoD Dominions\n");
	fprintf(allCountriesFile, "dynamic_tags = yes # any tags after this is considered dynamic dominions\n");
	for (std::map<std::string, V2Country*>::const_iterator i = dynamicCountries.begin(); i != dynamicCountries.end(); ++i)
	{
		i->second->outputToCommonCountriesFile(allCountriesFile);
	}
	fclose(allCountriesFile);

	// Create flags for all new countries.
	LOG(LogLevel::Info) << "-> Creating Flags";
	Flags flags;
	LOG(LogLevel::Info) << "-> Setting Flags";
	flags.setV2Tags(countries, countryMapper);
	LOG(LogLevel::Info) << "<- Writing Flags";
	flags.output();

	// Create localisations for all new countries. We don't actually know the names yet so we just use the tags as the names.
	LOG(LogLevel::Info) << "<- Writing Localisation Text";
	std::string localisationPath = "output/" + theConfiguration.getOutputName() + "/localisation";
	if (!Utils::TryCreateFolder(localisationPath))
	{
		return;
	}
	std::string source = theConfiguration.getVic2Path() + "/localisation/text.csv";
	std::string dest = localisationPath + "/text.csv";

	if (isRandomWorld)
	{
		LOG(LogLevel::Info) << "It's a random world";
		// we need to strip out the existing country names from the localisation file
		std::ifstream sourceFile(source);
		std::ofstream targetFile(dest);

		std::string line;
		std::regex countryTag("^[A-Z][A-Z][A-Z];");
		std::regex rebels("^REB;");
		std::smatch match;
		while (std::getline(sourceFile, line))
		{
			if (std::regex_search(line, match, countryTag) && !std::regex_search(line, match, rebels))
			{
				continue;
			}

			targetFile << line << '\n';
		}
		sourceFile.close();
		targetFile.close();

		// ...and also empty out 0_Names.csv
		FILE* zeronamesfile;
		std::string zeronamesfilepath = localisationPath + "/0_Names.csv";
		if (fopen_s(&zeronamesfile, zeronamesfilepath.c_str(), "w") != 0)
			fclose(zeronamesfile);
	}

	LOG(LogLevel::Info) << "<- Writting Localisation Names";
	std::ofstream localisationFile(localisationPath + "/0_Names.csv", std::ofstream::app);
	if (!localisationFile.is_open())
	{
		throw(std::runtime_error("Could not update localisation text file"));
	}

	Utils::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/history/countries");
	Utils::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/history/units");
	for (auto country: countries)
	{
		if (country.second->isNewCountry())
		{
			country.second->outputLocalisation(localisationFile);
		}
	}
	localisationFile.close();

	LOG(LogLevel::Info) << "<- Writing Provinces";
	Utils::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/history/provinces");
	for (auto province: provinces)
	{
		auto filename = province.second->getFilename();
		int lastSlash = filename.find_last_of('/');
		std::string path = filename.substr(0, lastSlash);
		Utils::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/history/provinces" + path);

		std::ofstream output("output/" + theConfiguration.getOutputName() + "/history/provinces" + filename);
		if (!output.is_open())
		{
			throw std::runtime_error("Could not create province history file output/" + theConfiguration.getOutputName() + "/history/provinces/" + filename + " - " + Utils::GetLastErrorString());
		}
		output << *province.second;
		output.close();
	}
	LOG(LogLevel::Info) << "<- Writing Countries";
	for (std::map<std::string, V2Country*>::const_iterator itr = countries.begin(); itr != countries.end(); ++itr)
	{
		itr->second->output();
	}
	LOG(LogLevel::Info) << "<- Writting Diplomacy";
	diplomacy.output();

	LOG(LogLevel::Info) << "<- Writting Pops";
	outputPops();

	// verify countries got written
	LOG(LogLevel::Info) << "-> Verifying All Countries Written";
	std::ifstream V2CountriesInput;
	V2CountriesInput.open(("output/" + theConfiguration.getOutputName() + "/common/countries.txt").c_str());
	if (!V2CountriesInput.is_open())
	{
		LOG(LogLevel::Error) << "Could not open countries.txt";
		exit(1);
	}

	bool	staticSection = true;
	while (!V2CountriesInput.eof())
	{
		std::string line;
		getline(V2CountriesInput, line);

		if ((line[0] == '#') || (line.size() < 3))
		{
			continue;
		}
		else if (line.substr(0, 12) == "dynamic_tags")
		{
			continue;
		}

		std::string countryFileName;
		int start = line.find_first_of('/');
		int size = line.find_last_of('\"') - start - 1;
		countryFileName = line.substr(start + 1, size);

		if (Utils::DoesFileExist("output/" + theConfiguration.getOutputName() + "/common/countries/" + countryFileName))
		{
		}
		else if (Utils::DoesFileExist(theConfiguration.getVic2Path() + "/common/countries/" + countryFileName))
		{
		}
		else
		{
			LOG(LogLevel::Warning) << "common/countries/" << countryFileName << " does not exists. This will likely crash Victoria 2.";
			continue;
		}
	}
	V2CountriesInput.close();
}

void V2::V2World::createModFile() const
{
	std::ofstream modFile("output/" + theConfiguration.getOutputName() + ".mod");
	if (!modFile.is_open())
	{
		LOG(LogLevel::Error) << "Could not create " << theConfiguration.getOutputName() << ".mod";
		exit(-1);
	}
	LOG(LogLevel::Info) << "\t-> Writing to: " << "output/" + theConfiguration.getOutputName() + ".mod";

	modFile << "name = \"Converted - " << theConfiguration.getOutputName() << "\"\n";
	modFile << "path = \"mod/" << theConfiguration.getOutputName() << "\"\n";
	modFile << "user_dir = \"" << theConfiguration.getOutputName() << "\"\n";
	modFile << "replace = \"history/provinces\"\n";
	modFile << "replace = \"history/countries\"\n";
	modFile << "replace = \"history/diplomacy\"\n";
	modFile << "replace = \"history/units\"\n";
	modFile << "replace = \"history/pops/1836.1.1\"\n";
	modFile << "replace = \"common/religion.txt\"\n";
	modFile << "replace = \"common/cultures.txt\"\n";
	modFile << "replace = \"common/countries.txt\"\n";
	modFile << "replace = \"common/countries/\"\n";
	modFile << "replace = \"gfx/interface/icon_religion.dds\"\n";
	modFile << "replace = \"localisation/0_Names.csv\"\n";
	modFile << "replace = \"localisation/0_Cultures.csv\"\n";
	modFile << "replace = \"localisation/0_Religions.csv\"\n";
	modFile << "replace = \"history/wars\"\n";

	modFile.close();
}



void V2::V2World::outputPops() const
{
	for (auto popRegion : popRegions)
	{
		std::ofstream popsFile;
		popsFile.open("output/" + theConfiguration.getOutputName() + "/history/pops/1836.1.1/" + popRegion.first);
		if (!popsFile.is_open())
		{
			throw std::runtime_error("Could not create pops file output/" + theConfiguration.getOutputName() + "/history/pops/1836.1.1/" + popRegion.first);
		}

		for (auto provinceNumber : popRegion.second)
		{
			const auto& provItr = provinces.find(provinceNumber);
			if (provItr != provinces.end())
			{
				popsFile << provItr->second->getPopsForOutput();
			}
			else
			{
				LOG(LogLevel::Error) << "Could not find province " << provinceNumber << " while outputing pops!";
			}
		}
	}
}

std::shared_ptr<V2::Province> V2::V2World::getProvince(const int provNum) const
{
	std::map<int, std::shared_ptr<V2::Province>>::const_iterator i = provinces.find(provNum);
	return (i != provinces.end()) ? i->second : nullptr;
}

V2::V2Country* V2::V2World::getCountry(std::string tag) const
{
	std::map<std::string, V2Country*>::const_iterator i = countries.find(tag);
	return (i != countries.end()) ? i->second : nullptr;
}
