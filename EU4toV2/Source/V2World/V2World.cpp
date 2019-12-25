/*Copyright (c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/



#include "V2World.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <list>
#include <queue>
#include <cmath>
#include <cfloat>
#include <fstream>
#include "ParadoxParser8859_15.h"
#include "ParadoxParserUTF8.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "../Configuration.h"
#include "../EU4World/Continents.h"
#include "../EU4World/EU4Diplomacy.h"
#include "../EU4World/EU4Leader.h"
#include "../EU4World/EU4Relations.h"
#include "../EU4World/World.h"
#include "../EU4World/Provinces/EU4Province.h"
#include "../Helpers/TechValues.h"
#include "../Mappers/AdjacencyMapper.h"
#include "../Mappers/CountryMapping.h"
#include "../Mappers/CultureMapper.h"
#include "../Mappers/Ideas/IdeaEffectMapper.h"
#include "../Mappers/Ideas/TechGroupsMapper.h"
#include "../Mappers/MinorityPopMapper.h"
#include "../Mappers/ReligionMapper.h"
#include "BlockedTechSchools.h"
#include "StateMapper.h"
#include "V2Province.h"
#include "V2State.h"
#include "V2Relations.h"
#include "V2TechSchools.h"
#include "V2Army.h"
#include "V2Leader.h"
#include "V2Pop.h"
#include "V2Country.h"
#include "V2Reforms.h"
#include "V2Flags.h"
#include "V2LeaderTraits.h"
#include "Vic2CultureUnionMapper.h"



V2World::V2World(const EU4::world& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper, const mappers::TechGroupsMapper& techGroupsMapper)
{
	LOG(LogLevel::Info) << "Parsing Vicky2 data";
	importProvinces();
	importDefaultPops();
	//logPopsByCountry();
	findCoastalProvinces();
	importPotentialCountries();
	isRandomWorld = sourceWorld.isRandomWorld();

	initializeProvinceMapper();
	sourceWorld.checkAllProvincesMapped(*provinceMapper);
	mappers::CountryMappings::createMappings(sourceWorld, potentialCountries, *provinceMapper);

	LOG(LogLevel::Info) << "Converting world";
	initializeCultureMappers(sourceWorld);
	initializeReligionMapper(sourceWorld);
	convertCountries(sourceWorld, ideaEffectMapper);
	convertProvinces(sourceWorld);
	convertDiplomacy(sourceWorld);
	setupColonies();
	setupStates();
	convertUncivReforms(sourceWorld, techGroupsMapper);
	convertTechs(sourceWorld);
	allocateFactories(sourceWorld);
	setupPops(sourceWorld);
	addUnions();
	convertArmies(sourceWorld);
	checkForCivilizedNations();

	output();
}


void V2World::importProvinces()
{
	LOG(LogLevel::Info) << "Importing provinces";

	set<string> provinceFilenames = discoverProvinceFilenames();
	for (auto provinceFilename : provinceFilenames)
	{
		V2Province* newProvince = new V2Province(provinceFilename);
		provinces.insert(make_pair(newProvince->getNum(), newProvince));
	}

	if (Utils::DoesFileExist("./blankMod/output/localisation/text.csv"))
	{
		importProvinceLocalizations("./blankMod/output/localisation/text.csv");
	}
	else
	{
		importProvinceLocalizations((theConfiguration.getVic2Path() + "/localisation/text.csv"));
	}

        importProvinceClimates();
        importProvinceTerrains();
}


std::set<std::string> V2World::discoverProvinceFilenames()
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

void V2World::importProvinceClimates()
{
	string filename = theConfiguration.getVic2Path() + "/map/climate.txt";
	if (!Utils::DoesFileExist(filename))
	{
		LOG(LogLevel::Warning) << "Could not find file " << filename
		                       << ", will not load climates.";
		return;
	}

        auto climateObj = parser_8859_15::doParseFile(filename);
	std::vector keywords = {"mild_climate", "temperate_climate",
	                        "harsh_climate", "inhospitable_climate"};
	for (const auto& key : keywords)
	{
		auto objs = climateObj->getValue(key);
		if (objs.size() < 2)
		{
			LOG(LogLevel::Warning)
			    << "Found " << objs.size() << " objects with key "
			    << key << ", will not assign this climate.";
			continue;
		}
		auto provList = objs[1];
		for (int i = 0; i < provList->numTokens(); ++i)
		{
			auto provNum = provList->tokenAsInt(i);
			if (provNum == 0)
			{
				continue;
			}
			auto* prov = getProvince(provNum.value());
			if (prov == NULL)
			{
				continue;
			}
			prov->setClimate(key);
		}
	}
}

void V2World::importProvinceLocalizations(const string& file)
{
	ifstream read(file);

	while (read.good() && !read.eof())
	{
		string line;
		getline(read, line);
		if (isAProvinceLocalization(line))
		{
			int position = line.find_first_of(';');
			int num = stoi(line.substr(4, position - 4));
			string name = line.substr(position + 1, line.find_first_of(';', position + 1) - position - 1);

			auto provice = provinces.find(num);
			if (provice != provinces.end())
			{
				provice->second->setName(name);
			}
		}
	}

	read.close();
}

void V2World::importProvinceTerrains()
{
	string filename = "terrainData.txt";
	if (!Utils::DoesFileExist(filename))
	{
		LOG(LogLevel::Warning) << "Could not find " << filename
		                       << ", will not load terrain data.";
		return;
	}

        auto terrainObj = parser_8859_15::doParseFile(filename);
	if (terrainObj == NULL)
	{
		LOG(LogLevel::Warning) << "Could not parse " << filename
		                       << ", will not load terrain data.";
		return;
	}

	auto leaves = terrainObj->getLeaves();
	for (auto leaf : leaves)
	{
		std::string key = leaf->getKey();
		int provNum = atoi(key.c_str());
		auto* province = getProvince(provNum);
		if (province == NULL)
		{
			continue;
		}
		// Do not override terrain set in province files.
		if (!province->getTerrain().empty())
		{
			continue;
		}
		province->setTerrain(leaf->getLeaf());
	}
}

bool V2World::isAProvinceLocalization(const string& line)
{
	return (line.substr(0, 4) == "PROV") && (isdigit(line[4]));
}


void V2World::importDefaultPops()
{
	LOG(LogLevel::Info) << "Importing historical pops.";

	totalWorldPopulation = 0;

	set<string> filenames;
	Utils::GetAllFilesInFolder("./blankMod/output/history/pops/1836.1.1/", filenames);
	for (auto filename : filenames)
	{
		importPopsFromFile(filename);
	}
}


void V2World::importPopsFromFile(const string& filename)
{
	list<int> popProvinces;

	shared_ptr<Object> fileObj = parser_8859_15::doParseFile(("./blankMod/output/history/pops/1836.1.1/" + filename));
	vector<shared_ptr<Object>> provinceObjs = fileObj->getLeaves();
	for (auto provinceObj : provinceObjs)
	{
		int provinceNum = stoi(provinceObj->getKey());
		popProvinces.push_back(provinceNum);

		importPopsFromProvince(provinceObj);
	}

	popRegions.insert(make_pair(filename, popProvinces));
}


void V2World::importPopsFromProvince(shared_ptr<Object> provinceObj)
{
	int provinceNum = stoi(provinceObj->getKey());
	auto province = provinces.find(provinceNum);
	if (province == provinces.end())
	{
		LOG(LogLevel::Warning) << "Could not find province " << provinceNum << " for original pops.";
		return;
	}

	int provincePopulation = 0;
	int provinceSlavePopulation = 0;

	vector<shared_ptr<Object>> popObjs = provinceObj->getLeaves();
	for (auto popObj: popObjs)
	{
		V2Pop* newPop = new V2Pop(popObj);

		province->second->addOldPop(newPop);
		if (minorityPopMapper::matchMinorityPop(newPop))
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


void V2World::logPopsByCountry() const
{
	map<string, map<string, long int>> popsByCountry; // country, poptype, num

	set<string> filenames;
	Utils::GetAllFilesInFolder("./blankMod/output/history/pops/1836.1.1/", filenames);
	for (auto filename : filenames)
	{
		logPopsFromFile(filename, popsByCountry);
	}

	outputLog(popsByCountry);
}


void V2World::logPopsFromFile(string filename, map<string, map<string, long int>>& popsByCountry) const
{
	shared_ptr<Object> fileObj = parser_8859_15::doParseFile(("./blankMod/output/history/pops/1836.1.1/" + filename));

	vector<shared_ptr<Object>> provinceObjs = fileObj->getLeaves();
	for (auto provinceObj : provinceObjs)
	{
		logPopsInProvince(provinceObj, popsByCountry);
	}
}


void V2World::logPopsInProvince(shared_ptr<Object> provinceObj, map<string, map<string, long int>>& popsByCountry) const
{
	int provinceNum = stoi(provinceObj->getKey());
	auto province = provinces.find(provinceNum);
	if (province == provinces.end())
	{
		LOG(LogLevel::Warning) << "Could not find province " << provinceNum << " for original pops.";
		return;
	}

	auto countryPopItr = getCountryForPopLogging(province->second->getOwner(), popsByCountry);

	vector<shared_ptr<Object>> pops = provinceObj->getLeaves();
	for (auto pop : pops)
	{
		logPop(pop, countryPopItr);
	}
}


void V2World::logPop(shared_ptr<Object> pop, map<string, map<string, long int>>::iterator countryPopItr) const
{
	string popType = pop->getKey();
	auto possibleSizeStr = pop->getLeaf("size");
	if (possibleSizeStr)
	{
		int popSize = stoi(*possibleSizeStr);

		auto popItr = countryPopItr->second.find(pop->getKey());
		if (popItr == countryPopItr->second.end())
		{
			long int newPopSize = 0;
			pair<map<string, long int>::iterator, bool> newIterator = countryPopItr->second.insert(make_pair(popType, newPopSize));
			popItr = newIterator.first;
		}
		popItr->second += popSize;
	}
}


map<string, map<string, long int>>::iterator V2World::getCountryForPopLogging(string country, map<string, map<string, long int>>& popsByCountry) const
{
	auto countryPopItr = popsByCountry.find(country);
	if (countryPopItr == popsByCountry.end())
	{
		map<string, long int> newCountryPop;
		auto newIterator = popsByCountry.insert(make_pair(country, newCountryPop));
		countryPopItr = newIterator.first;
	}

	return countryPopItr;
}


void V2World::outputLog(const map<string, map<string, long int>>& popsByCountry) const
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


void V2World::findCoastalProvinces()
{
	LOG(LogLevel::Info) << "Finding coastal provinces.";
	shared_ptr<Object> positionsObj = parser_8859_15::doParseFile((theConfiguration.getVic2Path() + "/map/positions.txt"));
	if (positionsObj == nullptr)
	{
		LOG(LogLevel::Error) << "Could not parse file " << theConfiguration.getVic2Path() << "/map/positions.txt";
		exit(-1);
	}

	vector<shared_ptr<Object>> provinceObjs = positionsObj->getLeaves();
	for (auto provinceObj: provinceObjs)
	{
		determineIfProvinceIsCoastal(provinceObj);
	}
}


void V2World::determineIfProvinceIsCoastal(shared_ptr<Object> provinceObj)
{
	vector<shared_ptr<Object>> positionObj = provinceObj->getValue("building_position");
	if (positionObj.size() > 0)
	{
		vector<shared_ptr<Object>> navalBaseObj = positionObj[0]->getValue("naval_base");
		if (navalBaseObj.size() > 0)
		{
			int provinceNum = stoi(provinceObj->getKey());
			auto province = provinces.find(provinceNum);
			if (province != provinces.end())
			{
				province->second->setCoastal(true);
			}
		}
	}
}


void V2World::importPotentialCountries()
{
	LOG(LogLevel::Info) << "Getting potential countries";
	potentialCountries.clear();
	dynamicCountries.clear();

	ifstream V2CountriesInput;
	V2CountriesInput.open("./blankMod/output/common/countries.txt");
	if (!V2CountriesInput.is_open())
	{
		LOG(LogLevel::Error) << "Could not open countries.txt. The converter may be corrupted, try downloading it again.";
		exit(-1);
	}

	bool dynamicSection = false;
	while (!V2CountriesInput.eof())
	{
		string line;
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


void V2World::importPotentialCountry(const string& line, bool dynamicCountry)
{
	string tag = line.substr(0, 3);

	V2Country* newCountry = new V2Country(line, this, dynamicCountry);
	potentialCountries.insert(make_pair(tag, newCountry));
	if (dynamicCountry)
	{
		dynamicCountries.insert(make_pair(tag, newCountry));
	}
}


void V2World::initializeCultureMappers(const EU4::world& sourceWorld)
{
	LOG(LogLevel::Info) << "Parsing culture mappings";

	std::ifstream cultureMapFile("cultureMap.txt");
	cultureMapper = std::make_unique<mappers::CultureMapper>(cultureMapFile);
	cultureMapFile.close();

	std::ifstream slaveCultureMapFile("slaveCultureMap.txt");
	slaveCultureMapper = std::make_unique<mappers::CultureMapper>(slaveCultureMapFile);
	slaveCultureMapFile.close();

	sourceWorld.checkAllEU4CulturesMapped(*cultureMapper);
}


void V2World::initializeReligionMapper(const EU4::world& sourceWorld)
{
	LOG(LogLevel::Info) << "Parsing religion mappings";

	std::ifstream mappingsFile("religionMap.txt");
	religionMapper = std::make_unique<mappers::ReligionMapper>(mappingsFile);
	mappingsFile.close();

	sourceWorld.checkAllEU4ReligionsMapped(*religionMapper);
}


void V2World::initializeProvinceMapper()
{
	LOG(LogLevel::Info) << "Parsing province mappings";
	std::ifstream mappingsFile("province_mappings.txt");
	provinceMapper = std::make_unique<mappers::ProvinceMapper>(mappingsFile, theConfiguration);
	mappingsFile.close();
}


void V2World::convertCountries(const EU4::world& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper)
{
	LOG(LogLevel::Info) << "Converting countries";
	initializeCountries(sourceWorld, ideaEffectMapper);
	convertNationalValues(ideaEffectMapper);
	convertPrestige();
	addAllPotentialCountries();
}


void V2World::initializeCountries(const EU4::world& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper)
{
	Vic2::blockedTechSchoolsFile theBlockedTechSchoolsFile;
	Vic2::TechSchoolsFile theTechSchoolsFile(theBlockedTechSchoolsFile.takeBlockedTechSchools());
	auto theTechSchools = theTechSchoolsFile.takeTechSchools();

	for (auto sourceCountry: sourceWorld.getCountries())
	{
		const string& V2Tag = mappers::CountryMappings::getVic2Tag(sourceCountry.first);
		if (V2Tag == "")
		{
			LOG(LogLevel::Error) << "EU4 tag " << sourceCountry.first << " is unmapped and cannot be converted.";
			exit(-1);
		}

		V2Country* destCountry = createOrLocateCountry(V2Tag, sourceCountry.second);
		destCountry->initFromEU4Country(
			sourceWorld.getRegions(),
			sourceCountry.second,
			theTechSchools,
			leaderIDMap,
			*cultureMapper,
			*slaveCultureMapper,
			ideaEffectMapper,
			*religionMapper,
			*provinceMapper
		);
		countries.insert(make_pair(V2Tag, destCountry));
	}
}


V2Country* V2World::createOrLocateCountry(const string& V2Tag, const shared_ptr<EU4::Country> sourceCountry)
{
	V2Country* destCountry = nullptr;

	auto potentialCountry = potentialCountries.find(V2Tag);
	if (potentialCountry == potentialCountries.end())
	{
		string countryFileName = sourceCountry->getName() + ".txt";
		destCountry = new V2Country(V2Tag, countryFileName, this);
	}
	else
	{
		destCountry = potentialCountry->second;
	}

	return destCountry;
}


bool scoresSorter(pair<V2Country*, double> first, pair<V2Country*, double> second)
{
	return (first.second > second.second);
}


void V2World::convertNationalValues(const mappers::IdeaEffectMapper& ideaEffectMapper)
{
	// set national values
	list<pair<V2Country*, double>> libertyScores;
	list<pair<V2Country*, double>> equalityScores;
	set<V2Country*>					valuesUnset;
	for (map<string, V2Country*>::iterator countryItr = countries.begin(); countryItr != countries.end(); countryItr++)
	{
		double libertyScore;
		double equalityScore;
		double orderScore;
		std::tie(libertyScore, equalityScore, orderScore) = countryItr->second->getNationalValueScores();

		if (libertyScore > orderScore)
		{
			libertyScores.push_back(make_pair(countryItr->second, libertyScore));
		}
		if ((equalityScore > orderScore) && (equalityScore > libertyScore))
		{
			equalityScores.push_back(make_pair(countryItr->second, equalityScore));
		}
		valuesUnset.insert(countryItr->second);
	}
	equalityScores.sort(scoresSorter);
	int equalityLeft = 5;
	for (list< pair<V2Country*, double> >::iterator equalItr = equalityScores.begin(); equalItr != equalityScores.end(); ++equalItr)
	{
		if (equalityLeft < 1)
		{
			break;
		}
		set<V2Country*>::iterator unsetItr = valuesUnset.find(equalItr->first);
		if (unsetItr != valuesUnset.end())
		{
			valuesUnset.erase(unsetItr);
			equalItr->first->setNationalValue("nv_equality");
			equalityLeft--;
			LOG(LogLevel::Debug) << "Assigning NV equality to country: " << equalItr->first->getTag() << ", equality left: " << equalityLeft;
		}
	}
	libertyScores.sort(scoresSorter);
	int libertyLeft = 20;
	for (list< pair<V2Country*, double> >::iterator libItr = libertyScores.begin(); libItr != libertyScores.end(); ++libItr)
	{
		if (libertyLeft < 1)
		{
			break;
		}
		set<V2Country*>::iterator unsetItr = valuesUnset.find(libItr->first);
		if (unsetItr != valuesUnset.end())
		{
			valuesUnset.erase(unsetItr);
			libItr->first->setNationalValue("nv_liberty");
			libertyLeft--;
			LOG(LogLevel::Debug) << "Assigning NV liberty to country: " << libItr->first->getTag() << ", liberty left: " << libertyLeft;
		}
	}
	for (set<V2Country*>::iterator unsetItr = valuesUnset.begin(); unsetItr != valuesUnset.end(); unsetItr++)
	{
		(*unsetItr)->setNationalValue("nv_order");
		LOG(LogLevel::Debug) << "Assigning NV order to country: " << (*unsetItr)->getTag();
	}
}


void V2World::convertPrestige()
{
	LOG(LogLevel::Debug) << "Setting prestige";

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


void V2World::addAllPotentialCountries()
{
	// ALL potential countries should be output to the file, otherwise some things don't get initialized right when loading Vic2
	for (auto potentialCountry : potentialCountries)
	{
		map<string, V2Country*>::iterator citr = countries.find(potentialCountry.first);
		if (citr == countries.end())
		{
			potentialCountry.second->initFromHistory();
			countries.insert(make_pair(potentialCountry.first, potentialCountry.second));
		}
	}
}


void V2World::checkForCivilizedNations()
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

	if (numPotentialGPs < 8)
	{
		LOG(LogLevel::Info) << "There were only " << numPotentialGPs << " civilized nations with more than 1 state. Attempting to editing defines.lua to reduce the number of great powers.";
		editDefines(numPotentialGPs);
	}
}

void V2World::editDefines(int numCivilisedNations)
{
	string greatNationsCount = "8";
	LOG(LogLevel::Info) << "Parsing defines.lua";
	shared_ptr<Object> definesObj = parser_UTF8::doParseFile("blankmod/output/common/defines.lua");
	if (definesObj == nullptr)
	{
		LOG(LogLevel::Error) << "Could not parse file defines.lua";
		exit(-1);
	}
	vector<shared_ptr<Object>> newDefinesObj = definesObj->getValue("defines");
	vector<shared_ptr<Object>> countryObj = newDefinesObj[0]->getValue("country");
	if (countryObj.size() > 0)
	{
		vector<shared_ptr<Object>> countryLeaves = countryObj[0]->getLeaves();
		for (unsigned int j = 0; j < countryLeaves.size(); j++)
		{
			string keyCoun = countryLeaves[j]->getKey();						// the key

			if (keyCoun == "GREAT_NATIONS_COUNT")
			{
				greatNationsCount = numCivilisedNations;
				countryLeaves[j]->setValue(greatNationsCount);					// sets the number of GPs = number of civilised nations
			}
			else
			{
				continue;
			}
		}
	}
	else
	{
		LOG(LogLevel::Warning) << "Invalid file structure for defines.lua.  You should edit this file yourself in [yourmod]/common";
	}
}


struct MTo1ProvinceComp
{
	vector<const EU4::Province*> provinces;
};


void V2World::convertProvinces(const EU4::world& sourceWorld)
{
	LOG(LogLevel::Info) << "Converting provinces";

	for (auto Vic2Province : provinces)
	{
		auto EU4ProvinceNumbers = provinceMapper->getEU4ProvinceNumbers(Vic2Province.first);
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
			provinceMapper->isProvinceResettable(Vic2Province.first, "resettableRegion")
		) {
			Vic2Province.second->setResettable(true);
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

		const std::string& V2ControllerTag = mappers::CountryMappings::getVic2Tag(oldControllerTag);
		const std::string& V2OwnerTag = mappers::CountryMappings::getVic2Tag(oldOwnerTag);
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
			std::map<string, V2Country*>::iterator ownerItr = countries.find(V2OwnerTag);
			if (ownerItr != countries.end())
			{
				ownerItr->second->addProvince(Vic2Province.second);
			}
			Vic2Province.second->convertFromOldProvince(
				sourceWorld.getAllReligions(),
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

						const std::string& coreV2Tag = mappers::CountryMappings::getVic2Tag(oldCore);
						if (!coreV2Tag.empty())
						{
							Vic2Province.second->addCore(coreV2Tag);
						}
					}

					// determine demographics
					double provPopRatio = sourceProvince->getBaseTax() / newProvinceTotalBaseTax;
					auto popRatios = sourceProvince->getPopRatios();
					std::vector<V2Demographic> demographics = determineDemographics(
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


std::vector<V2Demographic> V2World::determineDemographics(
	const EU4::Regions& eu4Regions,
	std::vector<EU4::PopRatio>& popRatios,
	const EU4::Province* eProv,
	V2Province* vProv,
	std::string oldOwnerTag,
	int destNum,
	double provPopRatio
)
{
	vector<V2Demographic> demographics;
	for (auto popRatio: popRatios)
	{
		std::optional<std::string> dstCulture;
		dstCulture = cultureMapper->cultureMatch(
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

		std::optional<std::string> religion = religionMapper->getVic2Religion(popRatio.getReligion());
		if (!religion)
		{
			LOG(LogLevel::Warning) << "Could not set religion for pops in Vic2 province " << destNum;
			religion = "";
		}

		std::optional<std::string> slaveCulture;
		slaveCulture = slaveCultureMapper->cultureMatch(
			eu4Regions,
			popRatio.getCulture(),
			popRatio.getReligion(),
			eProv->getNum(),
			oldOwnerTag
		);
		if (!slaveCulture)
		{
			auto thisContinent = EU4::continents::getEU4Continent(eProv->getNum());
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

		V2Demographic demographic;
		demographic.culture = *dstCulture;
		demographic.slaveCulture = *slaveCulture;
		demographic.religion = *religion;
		demographic.upperRatio = popRatio.getUpperRatio() * provPopRatio;
		demographic.middleRatio = popRatio.getMiddleRatio() * provPopRatio;
		demographic.lowerRatio = popRatio.getLowerRatio() * provPopRatio;
		demographic.oldCountry = oldOwnerTag;
		demographic.oldProvince = eProv;

		if (theConfiguration.getDebug())
		{
			LOG(LogLevel::Info) << "EU4 Province " << eProv->getNum() << ", "
				<< "Vic2 Province " << vProv->getNum() << ", "
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

void V2World::convertDiplomacy(const EU4::world& sourceWorld)
{
	LOG(LogLevel::Info) << "Converting diplomacy";

	vector<EU4Agreement> agreements = sourceWorld.getDiplomaticAgreements();
	for (vector<EU4Agreement>::iterator itr = agreements.begin(); itr != agreements.end(); ++itr)
	{
		const std::string& EU4Tag1 = itr->country1;
		const std::string& V2Tag1 = mappers::CountryMappings::getVic2Tag(EU4Tag1);
		if (V2Tag1.empty())
		{
			continue;
		}
		const std::string& EU4Tag2 = itr->country2;
		const std::string& V2Tag2 = mappers::CountryMappings::getVic2Tag(EU4Tag2);
		if (V2Tag2.empty())
		{
			continue;
		}

		map<string, V2Country*>::iterator country1 = countries.find(V2Tag1);
		map<string, V2Country*>::iterator country2 = countries.find(V2Tag2);
		if (country1 == countries.end())
		{
			LOG(LogLevel::Warning) << "Vic2 country " << V2Tag1 << " used in diplomatic agreement doesn't exist";
			continue;
		}
		if (country2 == countries.end())
		{
			LOG(LogLevel::Warning) << "Vic2 country " << V2Tag2 << " used in diplomatic agreement doesn't exist";
			continue;
		}
		std::optional<V2Relations> r1 = country1->second->getRelations(V2Tag2);
		if (!r1)
		{
			r1 = V2Relations(V2Tag2);
			country1->second->addRelation(*r1);
		}
		std::optional<V2Relations> r2 = country2->second->getRelations(V2Tag1);
		if (!r2)
		{
			r2 = V2Relations(V2Tag1);
			country2->second->addRelation(*r2);
		}

		if (itr->type == "is_colonial"|| itr->type == "colony")
		{
			country2->second->setColonyOverlord(country1->second);

			if (country2->second->getSourceCountry()->getLibertyDesire() < theConfiguration.getLibertyThreshold())
			{
				country1->second->absorbVassal(country2->second);
				for (vector<EU4Agreement>::iterator itr2 = agreements.begin(); itr2 != agreements.end(); ++itr2)
				{
					if (itr2->country2 == country2->second->getSourceCountry()->getTag())
					{
						itr2->country2 = country1->second->getSourceCountry()->getTag();
					}
				}
			}
			else
			{
				V2Agreement v2a;
				v2a.country1 = V2Tag1;
				v2a.country2 = V2Tag2;
				v2a.start_date = itr->startDate;
				v2a.type = "vassal";
				diplomacy.addAgreement(v2a);
				r1->setLevel(5);
			}
		}

		if ((itr->type == "is_march") || (itr->type == "march"))
		{
			country1->second->absorbVassal(country2->second);
			for (vector<EU4Agreement>::iterator itr2 = agreements.begin(); itr2 != agreements.end(); ++itr2)
			{
				if (itr2->country1 == country2->second->getSourceCountry()->getTag())
				{
					itr2->country1 = country1->second->getSourceCountry()->getTag();
				}
			}
		}

		if ((itr->type == "royal_marriage") || (itr->type == "guarantee"))
		{
			// influence level +1, but never exceed 4
			if (r1->getLevel() < 4)
			{
				r1->setLevel(r1->getLevel() + 1);
			}
		}
		if (itr->type == "royal_marriage")
		{
			// royal marriage is bidirectional; influence level +1, but never exceed 4
			if (r2->getLevel() < 4)
			{
				r2->setLevel(r2->getLevel() + 1);
			}
		}
		if ((itr->type == "vassal") || (itr->type == "client_vassal") || (itr->type == "daimyo_vassal") || (itr->type == "union") || (itr->type == "personal_union") || (itr->type == "protectorate") || (itr->type == "tributary_state"))
		{
			// influence level = 5
			r1->setLevel(5);
			/* FIXME: is this desirable?
			// if relations are too poor, country2 will immediately eject country1's ambassadors at the start of the game
			// so, for stability's sake, give their relations a boost
			if (r1->getRelations() < 1)
			r1->setRelations(1);
			if (r2->getRelations() < 1)
			r2->setRelations(1);
			*/
		}
		if ((itr->type == "alliance") || (itr->type == "vassal") || (itr->type == "client_vassal") || (itr->type == "daimyo_vassal") || (itr->type == "union") || (itr->type == "personal_union") || (itr->type == "guarantee"))
		{
			// copy agreement
			V2Agreement v2a;
			v2a.country1 = V2Tag1;
			v2a.country2 = V2Tag2;
			v2a.start_date = itr->startDate;
			v2a.type = itr->type;
			diplomacy.addAgreement(v2a);
		}
	}
}

void V2World::setupColonies()
{
	LOG(LogLevel::Info) << "Setting colonies";

	for (map<string, V2Country*>::iterator countryItr = countries.begin(); countryItr != countries.end(); countryItr++)
	{
		// find all land connections to capitals
		map<int, V2Province*>	openProvinces = provinces;
		queue<int>					goodProvinces;

		map<int, V2Province*>::iterator openItr = openProvinces.find(countryItr->second->getCapital());
		if (openItr == openProvinces.end())
		{
			continue;
		}
		if (openItr->second->getOwner() != countryItr->first) // if the capital is not owned, don't bother running
		{
			continue;
		}
		openItr->second->setLandConnection(true);
		goodProvinces.push(openItr->first);
		openProvinces.erase(openItr);

		do
		{
			int currentProvince = goodProvinces.front();
			goodProvinces.pop();
			auto adjacencies = mappers::adjacencyMapper::getVic2Adjacencies(currentProvince);
			if (adjacencies)
			{
				for (auto adjacency: *adjacencies)
				{
					auto openItr = openProvinces.find(adjacency);
					if (openItr == openProvinces.end())
					{
						continue;
					}
					if (openItr->second->getOwner() != countryItr->first)
					{
						continue;
					}
					openItr->second->setLandConnection(true);
					goodProvinces.push(openItr->first);
					openProvinces.erase(openItr);
				}
			}
		} while (goodProvinces.size() > 0);

		// find all provinces on the same continent as the owner's capital
		std::optional<std::string> capitalContinent;
		map<int, V2Province*>::iterator capital = provinces.find(countryItr->second->getCapital());
		if (capital != provinces.end())
		{
			const EU4::Province* capitalSrcProv = capital->second->getSrcProvince();
			if (!capitalSrcProv)
				continue;

			int capitalSrc = capitalSrcProv->getNum();
			capitalContinent = EU4::continents::getEU4Continent(capitalSrc);
			if (!capitalContinent)
			{
				continue;
			}
		}
		else
		{
			continue;
		}
		auto ownedProvinces = countryItr->second->getProvinces();
		for (auto provItr = ownedProvinces.begin(); provItr != ownedProvinces.end(); provItr++)
		{
			const EU4::Province* provSrcProv = provItr->second->getSrcProvince();
			if (!provSrcProv)
				continue;

			int provSrc = provSrcProv->getNum();
			std::optional<std::string> continent = EU4::continents::getEU4Continent(provSrc);
			if ((continent) && (continent == capitalContinent))
			{
				provItr->second->setSameContinent(true);
			}
		}
	}

	for (map<int, V2Province*>::iterator provItr = provinces.begin(); provItr != provinces.end(); provItr++)
	{
		provItr->second->determineColonial();
	}
}

static int stateId = 0;
void V2World::setupStates()
{
	LOG(LogLevel::Info) << "Creating states";
	list<V2Province*> unassignedProvs;
	for (map<int, V2Province*>::iterator itr = provinces.begin(); itr != provinces.end(); ++itr)
	{
		unassignedProvs.push_back(itr->second);
	}
	LOG(LogLevel::Debug) << "Unassigned Provs:\t" << unassignedProvs.size();

	Vic2::stateMapperFile theStateMapperFile;
	std::unique_ptr<Vic2::stateMapper> theStateMapper = theStateMapperFile.takeStateMapper();

	list<V2Province*>::iterator iter;
	while (unassignedProvs.size() > 0)
	{
		iter = unassignedProvs.begin();
		int		provId = (*iter)->getNum();
		string	owner = (*iter)->getOwner();

		if (owner == "")
		{
			unassignedProvs.erase(iter);
			continue;
		}

		V2State* newState = new V2State(stateId, *iter);
		stateId++;
		auto neighbors = theStateMapper->getAllProvincesInState(provId);

		bool colonial = (*iter)->isColonial();
		newState->setColonial(colonial);
		iter = unassignedProvs.erase(iter);

		for (auto i = neighbors.begin(); i != neighbors.end(); i++)
		{
			for (iter = unassignedProvs.begin(); iter != unassignedProvs.end(); iter++)
			{
				if ((*iter)->getNum() == *i)
				{
					if ((*iter)->getOwner() == owner)
					{
						if ((*iter)->isColonial() == colonial)
						{
							newState->addProvince(*iter);
							iter = unassignedProvs.erase(iter);
						}
					}
				}
			}
		}
		newState->colloectNavalBase();
		map<string, V2Country*>::iterator iter2 = countries.find(owner);
		if (iter2 != countries.end())
		{
			iter2->second->addState(newState);
		}
	}
}

void V2World::convertUncivReforms(const EU4::world& sourceWorld, const mappers::TechGroupsMapper& techGroupsMapper)
{
	LOG(LogLevel::Info) << "Setting unciv reforms";

	// tech group

	enum civConversion { older, newer };
	civConversion techGroupAlgoritm = newer;
	double topTech = 96;
	int topInstitutions = 7;
	auto version18 = EU4::Version("1.18.0");
	if (sourceWorld.getVersion() >= version18)
	{
		LOG(LogLevel::Info) << "New tech group conversion method";
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
		LOG(LogLevel::Info) << "Old tech group conversion method";
		techGroupAlgorithm = older;
	}

	for (map<string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
	{
		itr->second->convertUncivReforms(techGroupAlgorithm, topTech, topInstitutions, techGroupsMapper);
	}

	// inherit civilisation level for landless countries from their capital's owner
	for (map<string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
	{
		if (itr->second->getProvinces().size() == 0)
		{
			int capitalNum = itr->second->getCapital();
			if (capitalNum == 0)
				continue;
			V2Province* capital = getProvince(capitalNum);
			string capOwnerTag = capital->getOwner();
			V2Country* capOwner = getCountry(capOwnerTag);
			if (capOwner == nullptr)
				continue;
			itr->second->convertLandlessReforms(capOwner);
		}

	}
}


void V2World::convertTechs(const EU4::world& sourceWorld)
{
	LOG(LogLevel::Info) << "Converting techs";
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


void V2World::allocateFactories(const EU4::world& sourceWorld)
{
	// Construct factory factory
	LOG(LogLevel::Info) << "Determining factory allocation rules.";
	V2FactoryFactory factoryBuilder;

	LOG(LogLevel::Info) << "Allocating starting factories";

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
	deque<pair<double, V2Country*>> weightedCountries;
	for (map<string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
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
			weightedCountries.push_back(pair<double, V2Country*>(industryWeight, itr->second));
		}
	}
	if (weightedCountries.size() < 1)
	{
		LOG(LogLevel::Warning) << "No countries are able to accept factories";
		return;
	}
	sort(weightedCountries.begin(), weightedCountries.end());

	// allow a maximum of 10 (plus any tied at tenth place) countries to recieve factories
	deque<pair<double, V2Country*>> restrictCountries;
	double threshold = 1.0;
	double totalIndWeight = 0.0;
	for (deque<pair<double, V2Country*>>::reverse_iterator itr = weightedCountries.rbegin(); itr != weightedCountries.rend(); ++itr)
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
	deque<V2Factory*> factoryList = factoryBuilder.buildFactories();
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
	vector<pair<int, V2Country*>> factoryCounts;
	for (deque<pair<double, V2Country*>>::iterator itr = weightedCountries.begin(); itr != weightedCountries.end(); ++itr)
	{
		int factories = int(((itr->first / totalIndWeight) * factoryList.size()) + 0.5 /*round*/);
		LOG(LogLevel::Debug) << itr->second->getTag() << " has industrial weight " << itr->first << " granting max " << factories << " factories";
		factoryCounts.push_back(pair<int, V2Country*>(factories, itr->second));
	}

	// allocate the factories
	vector<pair<int, V2Country*>>::iterator lastReceptiveCountry = factoryCounts.end()--;
	vector<pair<int, V2Country*>>::iterator citr = factoryCounts.begin();
	while (factoryList.size() > 0)
	{
		bool accepted = false;
		if (citr->first > 0) // can take more factories
		{
			for (deque<V2Factory*>::iterator qitr = factoryList.begin(); qitr != factoryList.end(); ++qitr)
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
			Log logOutput(LogLevel::Debug);
			logOutput << "No countries will accept any of the remaining factories:\n";
			for (deque<V2Factory*>::iterator qitr = factoryList.begin(); qitr != factoryList.end(); ++qitr)
			{
				logOutput << "\t  " << (*qitr)->getTypeName() << '\n';
			}
			break;
		}
		if (++citr == factoryCounts.end())
		{
			citr = factoryCounts.begin(); // loop around to beginning
		}
	}
}

void V2World::setupPops(const EU4::world& sourceWorld)
{
	LOG(LogLevel::Info) << "Creating pops";

	long		my_totalWorldPopulation = static_cast<long>(0.55 * totalWorldPopulation);
	double	popWeightRatio = my_totalWorldPopulation / sourceWorld.getTotalProvinceWeights();

	//ofstream output_file("Data.csv");

	int popAlgorithm = 0;
	auto version12 = EU4::Version("1.12.0");
	if (sourceWorld.getVersion() >= version12)
	{
		LOG(LogLevel::Info) << "Using pop conversion algorithm for EU4 versions after 1.12.";
		popAlgorithm = 2;
	}
	else
	{
		LOG(LogLevel::Info) << "Using pop conversion algorithm for EU4 versions prior to 1.12.";
		popAlgorithm = 1;
	}

	for (map<string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
	{
		itr->second->setupPops(popWeightRatio, popAlgorithm, sourceWorld.getCountries(), *provinceMapper);
	}

	if (theConfiguration.getPopShaping() != Configuration::POPSHAPES::Vanilla)
	{
		LOG(LogLevel::Info) << "Total world population: " << my_totalWorldPopulation;
	}
	else
	{
		LOG(LogLevel::Info) << "Total world population: " << totalWorldPopulation;
	}
	LOG(LogLevel::Info) << "Total world weight sum: " << sourceWorld.getTotalProvinceWeights();
	LOG(LogLevel::Info) << my_totalWorldPopulation << " / " << sourceWorld.getTotalProvinceWeights();
	LOG(LogLevel::Info) << "Population per weight point is: " << popWeightRatio;

	long newTotalPopulation = 0;
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
	output_file << "POPS"		<< endl;*/
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

	//output_file.close();
}


void V2World::addUnions()
{
	LOG(LogLevel::Info) << "Adding unions";

	Vic2::CultureUnionMapperFile theVic2CultureUnionMapperFile;
	auto theVic2CultureUnionMapper = theVic2CultureUnionMapperFile.takeCultureUnionMapper();
	auto theVic2NationalsMapper = theVic2CultureUnionMapperFile.takeNationalsMapper();

	for (map<int, V2Province*>::iterator provItr = provinces.begin(); provItr != provinces.end(); provItr++)
	{
		if (!provItr->second->wasInfidelConquest() && !provItr->second->wasColony())
		{
			auto cultures = provItr->second->getCulturesOverThreshold(0.5);
			for (auto culture : cultures)
			{
				vector<string> unionCores = theVic2CultureUnionMapper->getCoreForCulture(culture);
				vector<string> nationalCores = theVic2NationalsMapper->getCoreForCulture(culture);
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
				}
			}
		}
	}
}


//#define TEST_V2_PROVINCES
void V2World::convertArmies(const EU4::world& sourceWorld)
{
	LOG(LogLevel::Info) << "Converting armies and navies";

	// hack for naval bases.  not ALL naval bases are in port provinces, and if you spawn a navy at a naval base in
	// a non-port province, Vicky crashes....
	vector<int> port_whitelist;
	{
		int temp = 0;
		ifstream s("port_whitelist.txt");
		while (s.good() && !s.eof())
		{
			s >> temp;
			port_whitelist.push_back(temp);
		}
		s.close();
	}

	// get cost per regiment values
	double cost_per_regiment[num_reg_categories] = { 0.0 };
	shared_ptr<Object>	obj2 = parser_8859_15::doParseFile("regiment_costs.txt");
	if (obj2 == nullptr)
	{
		LOG(LogLevel::Error) << "Could not parse file regiment_costs.txt";
		exit(-1);
	}
	vector<shared_ptr<Object>> objTop = obj2->getLeaves();
	if (objTop.size() == 0 || objTop[0]->getLeaves().size() == 0)
	{
		LOG(LogLevel::Error) << "regment_costs.txt failed to parse";
		exit(1);
	}
	for (int i = 0; i < num_reg_categories; ++i)
	{
		auto possibleRegimentCost = objTop[0]->getLeaf(RegimentCategoryNames[i]);
		if (possibleRegimentCost)
		{
			cost_per_regiment[i] = stoi(*possibleRegimentCost);
		}
	}

	// convert armies
	for (map<string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
	{
		itr->second->convertArmies(leaderIDMap, cost_per_regiment, provinces, port_whitelist, *provinceMapper);
	}
}

void V2World::output() const
{
	LOG(LogLevel::Info) << "Outputting mod";
	Utils::copyFolder("blankMod/output", "output/output");
	Utils::renameFolder("output/output", "output/" + theConfiguration.getOutputName());
	createModFile();

	// Record converter version

	LOG(LogLevel::Debug) << "Writing version";
	ofstream versionFile;

	try
	{
		versionFile.open("output/" + theConfiguration.getOutputName() + "/eu4tov2_version.txt");
		versionFile << "# 1.0J-prerelease \"Jan Mayen\", built on " << __TIMESTAMP__ << ".\n";
		versionFile.close();
	}
	catch (const std::exception&)
	{
		LOG(LogLevel::Error) << "Error writing version file! Is the output folder writeable?";
	}

	// Update bookmark starting dates

	ostringstream incomingDefines, incomingBookmarks;
	ifstream defines_lua("output/" + theConfiguration.getOutputName() + "/common/defines.lua");
	ifstream bookmarks_txt("output/" + theConfiguration.getOutputName() + "/common/bookmarks.txt");
	incomingDefines << defines_lua.rdbuf();
	incomingBookmarks << bookmarks_txt.rdbuf();
	defines_lua.close();
	bookmarks_txt.close();
	string startDate = "<STARTDATE>";
	string strDefines = incomingDefines.str();
	string strBookmarks = incomingBookmarks.str();
	size_t pos1 = strDefines.find(startDate);
	size_t pos2 = strBookmarks.find(startDate);
	strDefines.replace(pos1, startDate.length(), theConfiguration.getLastEU4Date().toString());
	strBookmarks.replace(pos2, startDate.length(), theConfiguration.getLastEU4Date().toString());

	ofstream out_defines_lua("output/" + theConfiguration.getOutputName() + "/common/defines.lua");
	ofstream out_bookmarks_txt("output/" + theConfiguration.getOutputName() + "/common/bookmarks.txt");
	out_defines_lua << strDefines;
	out_bookmarks_txt << strBookmarks;
	out_defines_lua.close();
	out_bookmarks_txt.close();

	// Create common\countries path.
	string countriesPath = "output/" + theConfiguration.getOutputName() + "/common/countries";
	if (!Utils::TryCreateFolder(countriesPath))
	{
		return;
	}

	// Output common\countries.txt
	LOG(LogLevel::Debug) << "Writing countries file";
	FILE* allCountriesFile;
	if (fopen_s(&allCountriesFile, ("output/" + theConfiguration.getOutputName() + "/common/countries.txt").c_str(), "w") != 0)
	{
		LOG(LogLevel::Error) << "Could not create countries file";
		exit(-1);
	}
	for (map<string, V2Country*>::const_iterator i = countries.begin(); i != countries.end(); i++)
	{
		const V2Country& country = *i->second;
		map<string, V2Country*>::const_iterator j = dynamicCountries.find(country.getTag());
		if (j == dynamicCountries.end())
		{
			country.outputToCommonCountriesFile(allCountriesFile);
		}
	}
	fprintf(allCountriesFile, "\n");
	if ((theConfiguration.getVic2Gametype() == "HOD") || (theConfiguration.getVic2Gametype() == "HoD_NNM"))
	{
		fprintf(allCountriesFile, "##HoD Dominions\n");
		fprintf(allCountriesFile, "dynamic_tags = yes # any tags after this is considered dynamic dominions\n");
		for (map<string, V2Country*>::const_iterator i = dynamicCountries.begin(); i != dynamicCountries.end(); i++)
		{
			i->second->outputToCommonCountriesFile(allCountriesFile);
		}
	}
	fclose(allCountriesFile);

	// Create flags for all new countries.
	V2Flags flags;
	flags.SetV2Tags(countries);
	flags.output();

	// Create localisations for all new countries. We don't actually know the names yet so we just use the tags as the names.
	LOG(LogLevel::Debug) << "Writing localisation text";
	string localisationPath = "output/" + theConfiguration.getOutputName() + "/localisation";
	if (!Utils::TryCreateFolder(localisationPath))
	{
		return;
	}
	string source = theConfiguration.getVic2Path() + "/localisation/text.csv";
	string dest = localisationPath + "/text.csv";

	if (isRandomWorld)
	{
		LOG(LogLevel::Debug) << "It's a random world";
		// we need to strip out the existing country names from the localisation file
		ifstream sourceFile(source);
		ofstream targetFile(dest);

		string line;
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
		string zeronamesfilepath = localisationPath + "/0_Names.csv";
		if (fopen_s(&zeronamesfile, zeronamesfilepath.c_str(), "w") != 0)
			fclose(zeronamesfile);
	}
	else
	{
		LOG(LogLevel::Debug) << "It's not a random world";
	}

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

	LOG(LogLevel::Debug) << "Writing provinces";
	Utils::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/history/provinces");
	for (auto province: provinces)
	{
		province.second->output();
	}
	LOG(LogLevel::Debug) << "Writing countries";
	for (map<string, V2Country*>::const_iterator itr = countries.begin(); itr != countries.end(); itr++)
	{
		itr->second->output();
	}
	diplomacy.output();

	outputPops();

	// verify countries got written
	ifstream V2CountriesInput;
	V2CountriesInput.open(("output/" + theConfiguration.getOutputName() + "/common/countries.txt").c_str());
	if (!V2CountriesInput.is_open())
	{
		LOG(LogLevel::Error) << "Could not open countries.txt";
		exit(1);
	}

	bool	staticSection = true;
	while (!V2CountriesInput.eof())
	{
		string line;
		getline(V2CountriesInput, line);

		if ((line[0] == '#') || (line.size() < 3))
		{
			continue;
		}
		else if (line.substr(0, 12) == "dynamic_tags")
		{
			continue;
		}

		string countryFileName;
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

void V2World::createModFile() const
{
	ofstream modFile("output/" + theConfiguration.getOutputName() + ".mod");
	if (!modFile.is_open())
	{
		LOG(LogLevel::Error) << "Could not create " << theConfiguration.getOutputName() << ".mod";
		exit(-1);
	}

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

void V2World::outputPops() const
{
	LOG(LogLevel::Debug) << "Writing pops";
	for (auto popRegion : popRegions)
	{
		FILE* popsFile;
		if (fopen_s(&popsFile, ("output/" + theConfiguration.getOutputName() + "/history/pops/1836.1.1/" + popRegion.first).c_str(), "w") != 0)
		{
			LOG(LogLevel::Error) << "Could not create pops file output/" << theConfiguration.getOutputName() << "/history/pops/1836.1.1/" << popRegion.first;
			exit(-1);
		}

		for (auto provinceNumber : popRegion.second)
		{
			map<int, V2Province*>::const_iterator provItr = provinces.find(provinceNumber);
			if (provItr != provinces.end())
			{
				provItr->second->outputPops(popsFile);
			}
			else
			{
				LOG(LogLevel::Error) << "Could not find province " << provinceNumber << " while outputing pops!";
			}
		}
	}
}

V2Province* V2World::getProvince(const int provNum) const
{
	map<int, V2Province*>::const_iterator i = provinces.find(provNum);
	return (i != provinces.end()) ? i->second : nullptr;
}

V2Country* V2World::getCountry(string tag) const
{
	map<string, V2Country*>::const_iterator i = countries.find(tag);
	return (i != countries.end()) ? i->second : nullptr;
}
