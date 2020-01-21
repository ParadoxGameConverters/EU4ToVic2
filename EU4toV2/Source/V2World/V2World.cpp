#include "V2World.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "../Mappers/Pops/PopMapper.h"
#include <fstream>
#include "../EU4World/World.h"
#include <queue>
#include "../Helpers/TechValues.h"
#include "Flags/Flags.h"
#include "../Mappers/VersionParser/VersionParser.h"

V2::World::World(const EU4::World& sourceWorld, 
	const mappers::IdeaEffectMapper& ideaEffectMapper, 
	const mappers::TechGroupsMapper& techGroupsMapper, 
	const mappers::VersionParser& versionParser)
{
	LOG(LogLevel::Info) << "*** Hello Vicky 2, creating world. ***";
	LOG(LogLevel::Info) << "-> Importing Provinces";
	importProvinces();
	
	LOG(LogLevel::Info) << "-> Importing Vanilla Pops";
	importDefaultPops();

	if (theConfiguration.getDebug()) countryPopLogger.logPopsByCountry(provinces);
	
	LOG(LogLevel::Info) << "-> Importing Potential Countries";
	importPotentialCountries();
	isRandomWorld = sourceWorld.isRandomWorld();

	LOG(LogLevel::Info) << "-> Loading Country Mapping Rules";
	countryMapper.createMappings(sourceWorld, potentialCountries, provinceMapper);

	LOG(LogLevel::Info) << "-> Loading Culture Mapping Rules";
	initializeCultureMappers(sourceWorld);
	mappingChecker.check(sourceWorld, provinceMapper, religionMapper, cultureMapper);
	
	LOG(LogLevel::Info) << "-> Converting Countries";
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
	convertArmies();

	LOG(LogLevel::Info) << "---> Le Dump <---";
	
	output(versionParser);
	LOG(LogLevel::Info) << "*** Goodbye, Vicky 2, and godspeed. ***";
}

void V2::World::importProvinces()
{
	auto provinceFilenames = discoverProvinceFilenames();
	for (const auto& provinceFilename : provinceFilenames)
	{
		auto newProvince = std::make_shared<Province>(provinceFilename, climateMapper, terrainDataMapper, provinceNameParser, navalBaseMapper);
		provinces.insert(std::make_pair(newProvince->getID(), newProvince));
	}

	if (theConfiguration.getRandomiseRgos())
	{
		shuffleRgos();
	}
}

std::set<std::string> V2::World::discoverProvinceFilenames()
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

void V2::World::shuffleRgos()
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


void V2::World::importDefaultPops()
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

void V2::World::importPopsFromFile(const std::string& filename, const mappers::MinorityPopMapper& minorityPopMapper)
{
	std::list<int> popProvinces;

	// We are using our own defaults instead of vanilla source because it was modded with cultural minorities.
	const mappers::PopMapper popMapper("./blankMod/output/history/pops/1836.1.1/" + filename);

	for (const auto& provinceItr : popMapper.getProvincePopTypeMap())
	{
		int provinceNum = provinceItr.first;
		popProvinces.push_back(provinceNum);

		importPopsFromProvince(provinceNum, provinceItr.second, minorityPopMapper);
	}
	popRegions.insert(std::make_pair(filename, popProvinces));
}

void V2::World::importPopsFromProvince(int provinceID, const mappers::PopTypes& popType, const mappers::MinorityPopMapper& minorityPopMapper)
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
		auto newPop = std::make_shared<Pop>(pop.first, pop.second.getSize(), pop.second.getCulture(), pop.second.getReligion());
		if (minorityPopMapper.blankMajorityFromMinority(*newPop))
		{
			// If the pop we loaded had minority elements, their majority elements are now blank.
			// Ie. We've removed yankee from mormons, and we'll fill them later dynamically to become <majorityculture> mormons.
			province->second->addMinorityPop(newPop);
		}
		province->second->addVanillaPop(newPop);

		totalWorldPopulation += newPop->getSize();
		provincePopulation += newPop->getSize();
		if (newPop->isSlavePop()) provinceSlavePopulation += newPop->getSize();
	}

	province->second->setSlaveProportion(1.0 * provinceSlavePopulation / provincePopulation);
}

void V2::World::importPotentialCountries()
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

void V2::World::importPotentialCountry(const std::string& line, bool dynamicCountry)
{
	std::string tag = line.substr(0, 3);

	auto newCountry = std::make_shared<Country>(line, dynamicCountry, partyNameMapper, partyTypeMapper);
	potentialCountries.insert(std::make_pair(tag, newCountry));
	if (dynamicCountry)
	{
		dynamicCountries.insert(std::make_pair(tag, newCountry));
	}
}

void V2::World::initializeCultureMappers(const EU4::World& sourceWorld)
{
	LOG(LogLevel::Info) << "Parsing culture mappings.";
	cultureMapper.loadFile("configurables/culture_map.txt");
	
	LOG(LogLevel::Info) << "Parsing slave culture mappings.";
	slaveCultureMapper.loadFile("configurables/culture_map_slaves.txt");
}

void V2::World::convertCountries(const EU4::World& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper)
{
	initializeCountries(sourceWorld, ideaEffectMapper);
	convertNationalValues(ideaEffectMapper);
	convertPrestige();
	addAllPotentialCountries();
}

void V2::World::initializeCountries(const EU4::World& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper)
{
	for (auto sourceCountry: sourceWorld.getCountries())
	{
		const auto& V2Tag = countryMapper.getV2Tag(sourceCountry.first);
		if (!V2Tag) throw std::runtime_error("EU4 tag " + sourceCountry.first + " is unmapped and cannot be converted.");

		std::shared_ptr<Country> destCountry = createOrLocateCountry(*V2Tag, sourceCountry.second);
		
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
		countries.insert(std::make_pair(*V2Tag, destCountry));
	}
}

std::shared_ptr<V2::Country> V2::World::createOrLocateCountry(const std::string& V2Tag, const std::shared_ptr<EU4::Country> sourceCountry)
{
	std::shared_ptr<Country> destCountry;

	const auto& potentialCountry = potentialCountries.find(V2Tag);
	if (potentialCountry == potentialCountries.end())
	{
		auto countryFileName = sourceCountry->getName() + ".txt";
		destCountry = std::make_shared<Country>(V2Tag, countryFileName, partyNameMapper, partyTypeMapper);
	}
	else
	{
		destCountry = potentialCountry->second;
	}
	return destCountry;
}

bool scoresSorter(std::pair<std::shared_ptr<V2::Country>, double> first, std::pair<std::shared_ptr<V2::Country>, double> second)
{
	return (first.second > second.second);
}

void V2::World::convertNationalValues(const mappers::IdeaEffectMapper& ideaEffectMapper)
{
	// set national values
	std::list<std::pair<std::shared_ptr<Country>, double>> libertyCountries;
	std::list<std::pair<std::shared_ptr<Country>, double>> equalityCountries;
	std::set<std::shared_ptr<Country>> countriesToSet;
	for (const auto& country: countries)
	{
		auto nvScores = country.second->getNationalValueScores();

		if (nvScores.libertyInvestment > nvScores.orderInvestment)
		{
			libertyCountries.emplace_back(std::make_pair(country.second, nvScores.libertyInvestment));
		}
		if (nvScores.equalityInvestment > nvScores.orderInvestment && nvScores.equalityInvestment > nvScores.libertyInvestment)
		{
			equalityCountries.emplace_back(std::make_pair(country.second, nvScores.equalityInvestment));
		}
		countriesToSet.insert(country.second);
	}
	equalityCountries.sort(scoresSorter);

	// hardcoded so 5 countries will get to be equality oriented. Not really worth the trouble to put in a config file.
	int equalityLeft = 5;
	for (const auto& equalCountry: equalityCountries)
	{
		if (equalityLeft < 1) break;
		const auto& countryToSetItr = countriesToSet.find(equalCountry.first);
		if (countryToSetItr != countriesToSet.end())
		{
			countriesToSet.erase(countryToSetItr);
			equalCountry.first->setNationalValue("nv_equality");
			equalityLeft--;
		}
	}

	// also hardcoded to 20 countries.
	libertyCountries.sort(scoresSorter);
	int libertyLeft = 20;
	for (const auto& libertyCountry: libertyCountries)
	{
		if (libertyLeft < 1) break;
		const auto& countryToSetItr = countriesToSet.find(libertyCountry.first);
		if (countryToSetItr != countriesToSet.end())
		{
			countriesToSet.erase(countryToSetItr);
			libertyCountry.first->setNationalValue("nv_liberty");
			libertyLeft--;
		}
	}

	// The rest will default anyway.
}

void V2::World::convertPrestige()
{
	LOG(LogLevel::Info) << "-> Setting prestige";

	double highestScore = 0;
	for (auto country: countries)
	{
		double score = 0;
		auto srcCountry = country.second->getSourceCountry();
		if (srcCountry) score = srcCountry->getScore();
		if (score > highestScore) highestScore = score;
	}

	for (auto country: countries)
	{
		double score = 0;
		double prestige = 0;
		auto srcCountry = country.second->getSourceCountry();
		if (srcCountry) score = srcCountry->getScore();
		if (highestScore > 0) prestige = score / highestScore * 100.0;
		country.second->addPrestige(prestige);
	}
}

void V2::World::addAllPotentialCountries()
{
	// ALL potential countries should be output to the file, otherwise some things don't get initialized right when loading Vic2
	for (auto potentialCountry : potentialCountries)
	{
		const auto& countryItr = countries.find(potentialCountry.first);
		if (countryItr == countries.end())
		{
			// Note: This is a dead country.
			potentialCountry.second->initFromHistory(unreleasablesMapper);
			countries.insert(make_pair(potentialCountry.first, potentialCountry.second));
		}
	}
}

unsigned int V2::World::countCivilizedNations() const
{
	unsigned int numPotentialGPs = 0;
	for (const auto& country : countries)
	{
		auto states = country.second->getStates();
		if (country.second->isCivilized() && states.size() > 1)
		{
			numPotentialGPs++;
		}
	}

	return numPotentialGPs;
}

void V2::World::convertProvinces(const EU4::World& sourceWorld)
{
	for (const auto& province : provinces)
	{
		auto eu4ProvinceNumbers = provinceMapper.getEU4ProvinceNumbers(province.first);
		if (eu4ProvinceNumbers.empty())
		{
			LOG(LogLevel::Warning) << "No mappings found for V2 province " << province.first << " (" << province.second->getName() << ")";
			// We leave it to defaults
			province.second->sterilizeProvince();
			continue;
		}
		if (std::count(eu4ProvinceNumbers.begin(), eu4ProvinceNumbers.end(), 0) > 0)
		{
			LOG(LogLevel::Warning) << "Invalid mapping found for V2 province " << province.first << " (" << province.second->getName() << ")";
			// We have a buggy mapping somewhere. Better drop.
			province.second->sterilizeProvince();
			continue;
		}
		if (theConfiguration.getResetProvinces() == "yes" && provinceMapper.isProvinceResettable(province.first, "resettableRegion")) 
		{
			// This province is supposed to get filled by nearby provinces, later, somewhere.
			// TODO: This functionality doesn't exist. We have vanilla pop output but that's broken.
			province.second->setResettable();
			continue;
		}

		auto eu4owner = determineProvinceOwnership(eu4ProvinceNumbers, sourceWorld);
		if (!eu4owner)
		{
			// Probably uncolonized province; blank it and leave it to defaults.
			province.second->sterilizeProvince();
			continue;
		}
			
		// Remap owner to something V2 can understand
		auto possibleOwner = countryMapper.getV2Tag(*eu4owner);
		if (!possibleOwner) throw std::runtime_error("Error mapping EU4 tag " + *eu4owner + " to a Vic2 tag!");
		auto owner = *possibleOwner;

		// TODO: Once we have support for importing current wars, assign control of province to actual controller.
		province.second->setOwner(owner);
		province.second->setController(owner);

		const auto& ownerCountry = countries.find(owner);
		if (ownerCountry != countries.end())
		{
			ownerCountry->second->addProvince(province.second);
		}

		// Before we convert a province, we need to filter those eu4 province sources belonging to another owner.
		// ... don't want to influence development with filthy foreign manufactories and forts.
		std::vector<std::shared_ptr<EU4::Province>> filteredSources;
		for (const auto& eu4provID: eu4ProvinceNumbers)
		{
			if (sourceWorld.getProvince(eu4provID)->getOwnerString() == *eu4owner)
			{
				filteredSources.push_back(sourceWorld.getProvince(eu4provID));
			}
		}
		
		province.second->convertFromOldProvince(
			filteredSources, 
			sourceWorld.getCountries(), 
			sourceWorld.getRegions(), 
			cultureMapper, 
			slaveCultureMapper, 
			continentsMapper, 
			religionMapper,
			countryMapper);
	}
}

std::optional<std::string> V2::World::determineProvinceOwnership(const std::vector<int>& eu4ProvinceNumbers, const EU4::World& sourceWorld) const
{
	// determine ownership by province development.
	std::map<std::string, std::vector<std::shared_ptr<EU4::Province>>> theClaims; // tag, claimed provinces
	std::map<std::string, std::pair<int, int>> theShares; // tag, development/tax

	for (auto eu4ProvinceID : eu4ProvinceNumbers)
	{
		const auto& eu4province = sourceWorld.getProvince(eu4ProvinceID);
		auto ownerTag = eu4province->getOwnerString();
		if (ownerTag.empty()) continue; // Don't touch uncolonized provinces.
		theClaims[ownerTag].push_back(eu4province);
		theShares[ownerTag] = std::make_pair(lround(eu4province->getTotalDevModifier()), lround(eu4province->getBaseTax()));
	}
	// Let's see who the lucky winner is.
	std::string winner; int maxDev = 0; int maxTax = 0;
	for (const auto& share : theShares)
	{
		if (share.second.first > maxDev)
		{
			winner = share.first; maxDev = share.second.first; maxTax = share.second.second;
		}
		if (share.second.first == maxDev && share.first != winner)
		{
			// We have a tie
			if (share.second.second > maxTax)
			{
				winner = share.first; maxDev = share.second.first; maxTax = share.second.second;
			}
			if (share.second.second == maxTax)
			{
				// Shit. Check for core?
				for (const auto& claim : theClaims[share.first])
					if (!claim->isTerritorialCore())
					{
						// It's a full core of someone. Might as well take it. Will not resolve further ties, thank you.
						winner = share.first; maxDev = share.second.first; maxTax = share.second.second;
					}
			}
		}
	}
	if (winner.empty()) return std::nullopt;
	return winner;
}

void V2::World::setupColonies()
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
			if (!adjacencies) continue;
			for (auto adjacency: *adjacencies)
			{
				auto openItr2 = openProvinces.find(adjacency);
				if (openItr2 == openProvinces.end()) continue;
				if (openItr2->second->getOwner() != countryItr.first) continue;
				openItr2->second->setLandConnection(true);
				goodProvinces.push(openItr2->first);
				openProvinces.erase(openItr2);
			}
		} while (!goodProvinces.empty());

		// find all provinces on the same continent as the owner's capital
		std::optional<std::string> capitalContinent;
		auto capital = provinces.find(countryItr.second->getCapital());
		if (capital != provinces.end())
		{
			auto capitalSources = capital->second->getEU4IDs();		
			capitalContinent = continentsMapper.getEU4Continent(*capitalSources.begin());
			if (!capitalContinent) continue;
		}
		else
		{
			continue;
		}
		auto ownedProvinces = countryItr.second->getProvinces();
		for (const auto& ownedProvince: ownedProvinces)
		{
			auto provinceSources = ownedProvince.second->getEU4IDs();
			auto continent = continentsMapper.getEU4Continent(*provinceSources.begin());
			if (continent && continent == capitalContinent)
			{
				ownedProvince.second->setSameContinent();
			}
		}
	}
}

void V2::World::setupStates()
{
	std::list<std::shared_ptr<Province>> unassignedProvs;
	for (const auto& province: provinces) unassignedProvs.push_back(province.second);

	while (!unassignedProvs.empty())
	{
		auto iter = unassignedProvs.begin();
		int provId = (*iter)->getID();
		std::string owner = (*iter)->getOwner();

		if (owner.empty())
		{
			unassignedProvs.erase(iter);
			continue;
		}

		auto newState = std::make_shared<State>(stateId, *iter);
		stateId++;
		auto neighbors = stateMapper.getAllProvincesInState(provId);

		// We are breaking states apart according to colonial status. This is so primitives can retain 
		// their full states next to colonizers who have colonial provinces in the same state.
		// This ALSO means multiple naval bases within apparently single state.
		auto colonial = (*iter)->isColony();
		newState->setColonial(colonial);
		iter = unassignedProvs.erase(iter);

		for (const auto& neighborID: neighbors)
		{
			for (iter = unassignedProvs.begin(); iter != unassignedProvs.end(); iter++)
			{
				if ((*iter)->getID() == neighborID)
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
		const auto& iter2 = countries.find(owner);
		if (iter2 != countries.end())
		{
			iter2->second->addState(newState, portProvincesMapper);
		}
	}
}

void V2::World::convertUncivReforms(const EU4::World& sourceWorld, const mappers::TechGroupsMapper& techGroupsMapper)
{
	// tech group	
	auto techGroupAlgorithm = CIV_ALGORITHM::newer;
	double topTech = 0;
	int topInstitutions = 0;
	auto version18 = EU4::Version("1.18.0");

	if (sourceWorld.getVersion() >= version18)
	{
		// Find global max tech and institutions embraced

		for (const auto& country: sourceWorld.getCountries())
		{
			if (country.second->getProvinces().empty()) continue;
			auto totalTechs = country.second->getMilTech() + country.second->getAdmTech() + country.second->getDipTech();
			if (totalTechs > topTech) topTech = totalTechs;
			auto currInstitutions = country.second->numEmbracedInstitutions();
			if (currInstitutions > topInstitutions) topInstitutions = currInstitutions;
		}
	}
	else
	{
		// Hardcoded for best monolithic experience.
		topTech = 96;
		topInstitutions = 7;
		techGroupAlgorithm = CIV_ALGORITHM::older;
	}

	for (const auto& country: countries)
	{
		country.second->convertUncivReforms(techGroupAlgorithm, topTech, topInstitutions, techGroupsMapper);
	}

	// inherit civilisation level for landless countries from their capital's owner
	for (const auto& country: countries)
	{
		if (!country.second->getProvinces().empty()) continue;
		auto capitalID = country.second->getCapital();
		if (capitalID == 0) continue;		
		const auto& capital = getProvince(capitalID);
		std::string capOwnerTag = capital->getOwner();
		std::shared_ptr<Country> capOwner = getCountry(capOwnerTag);
		if (!capOwner) continue;
		country.second->convertLandlessReforms(capOwner);
	}
}

void V2::World::convertTechs(const EU4::World& sourceWorld)
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


void V2::World::allocateFactories(const EU4::World& sourceWorld)
{
	// determine average production tech
	const auto& sourceCountries = sourceWorld.getCountries();
	double admMean = 0;
	int num = 1;
	for (const auto& country: sourceCountries)
	{
		if (country.second->getProvinces().empty()) continue;

		double admTech = country.second->getAdmTech();
		admMean += ((admTech - admMean) / num);
		++num;
	}

	// give all extant civilized nations an industrial score
	std::deque<std::pair<double, std::shared_ptr<Country>>> weightedCountries;
	for (const auto& country : countries)
	{
		if (!country.second->isCivilized()) continue;

		auto sourceCountry = country.second->getSourceCountry();
		if (!sourceCountry) continue;
		if (country.second->getProvinces().empty()) continue;

		// modified manufactory weight follows diminishing returns curve y = x^(3/4)+log((x^2)/5+1)
		int manuCount = sourceCountry->getManufactoryCount();
		double manuWeight = pow(manuCount, 0.75) + log1p(static_cast<double>(pow(manuCount, 2)) / 5.0);
		double industryWeight = sourceCountry->getAdmTech() - admMean + manuWeight;
		// having one manufactory and average tech is not enough; you must have more than one, or above-average tech
		if (industryWeight > 1.0)
		{
			weightedCountries.push_back(std::pair<double, std::shared_ptr<Country>>(industryWeight, country.second));
		}
	}

	if (weightedCountries.empty())
	{
		LOG(LogLevel::Warning) << "No countries are able to accept factories? What's the state of tech in this world?";
		return;
	}
	
	sort(weightedCountries.begin(), weightedCountries.end(), [](const std::pair<double, std::shared_ptr<Country>>& country1, const std::pair<double, std::shared_ptr<Country>>& country2)
	{
			return country1.first > country2.first;
	});

	// allow a maximum of 15 (plus any tied at fifteenth place) countries to receive factories
	std::deque<std::pair<double, std::shared_ptr<Country>>> restrictCountries;
	double threshold = 1.0;
	double totalIndWeight = 0.0;
	for (const auto& country : weightedCountries)
	{
		if (restrictCountries.size() > 15 && country.first < threshold - FLT_EPSILON) break;

		restrictCountries.push_back(country); // preserve sort
		totalIndWeight += country.first;
		threshold = country.first;
	}

	if (totalIndWeight == 0)
	{
		LOG(LogLevel::Warning) << "No factories for anyone! Industry levels are too unified - are you converting a starting date?";
		return;
	} 

	weightedCountries.swap(restrictCountries);

	// remove nations that won't have enough industrial score for even one factory
	std::deque<std::shared_ptr<Factory>> factoryList = factoryTypeMapper.buildFactories();
	while (lround(weightedCountries.begin()->first / totalIndWeight * factoryList.size()) < 1.0)
	{
		weightedCountries.pop_front();
		if (weightedCountries.empty())
		{
			LOG(LogLevel::Warning) << "These are all primitives! No factories for anyone!";
			return;
		}
	}

	// determine how many factories each eligible nation gets
	std::vector<std::pair<int, std::shared_ptr<Country>>> factoryCounts;
	for (const auto& country: weightedCountries)
	{
		int factories = lround(country.first / totalIndWeight * factoryList.size());
		factoryCounts.emplace_back(std::pair<int, std::shared_ptr<Country>>(factories, country.second));
	}

	// allocate the factories
	auto lastReceptiveCountry = factoryCounts.end()--;
	auto citr = factoryCounts.begin();
	while (!factoryList.empty())
	{
		bool accepted = false;
		if (citr->first > 0) // can take more factories
		{
			for (auto qitr = factoryList.begin(); qitr != factoryList.end(); ++qitr)
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

void V2::World::setupPops(const EU4::World& sourceWorld)
{
	long my_totalWorldPopulation = static_cast<long>(0.55 * totalWorldPopulation); // What is this? Why 0.55? Why not redistribute vanilla world population size?
	double popWeightRatio = my_totalWorldPopulation / sourceWorld.getTotalProvinceWeights(); // This is relevant only for extreme reshaping.

	CIV_ALGORITHM popAlgorithm;
	auto version12 = EU4::Version("1.12.0");
	if (sourceWorld.getVersion() >= version12)
	{
		popAlgorithm = CIV_ALGORITHM::newer;
	}
	else
	{
		popAlgorithm = CIV_ALGORITHM::older;
	}

	for (std::map<std::string, std::shared_ptr<Country>>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
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
}

void V2::World::addUnions()
{
	if (theConfiguration.getCoreHandling() == Configuration::COREHANDLES::DropAll) return;

	LOG(LogLevel::Info) << "Parsing cultural union mappings.";
	culturalUnionMapper.loadFile("configurables/unions.txt");
	LOG(LogLevel::Info) << "Parsing nationalities mappings.";
	culturalNationalitiesMapper.loadFile("configurables/nationals.txt");

	LOG(LogLevel::Info) << "Distributing national and cultural union cores.";

	for (const auto& province: provinces)
	{
		if (!province.second->wasColony())
		{
			auto cultures = province.second->getCulturesOverThreshold(0.5);
			for (const auto& culture : cultures)
			{
				auto unionCores = culturalUnionMapper.getCoresForCulture(culture);
				auto nationalCores = culturalNationalitiesMapper.getCoresForCulture(culture);
				switch (theConfiguration.getCoreHandling())
				{
				case Configuration::COREHANDLES::DropNational:
					if (!unionCores) break;
					for (const auto& core : *unionCores)
					{
						province.second->addCore(core);
					}
					break;
				case Configuration::COREHANDLES::DropUnions:
					if (!nationalCores) break;
					for (const auto& core : *nationalCores)
					{
						province.second->addCore(core);
					}
					break;
				case Configuration::COREHANDLES::DropNone:
					if (!unionCores) break;
					for (const auto& core : *unionCores)
					{
						province.second->addCore(core);
					}
					if (!nationalCores) break;
					for (const auto& core : *nationalCores)
					{
						province.second->addCore(core);
					}
				case Configuration::COREHANDLES::DropAll:
				default:
					break;
				}
			}
		}
	}
}


void V2::World::convertArmies()
{
	// convert leaders and armies
	for (const auto& country: countries)
	{
		country.second->convertLeaders(leaderTraitMapper);
		country.second->convertArmies(regimentCostsMapper, provinces, portProvincesMapper, provinceMapper);
	}
}

void V2::World::output(const mappers::VersionParser& versionParser) const
{
	LOG(LogLevel::Info) << "<- Copying Mod Template";
	Utils::copyFolder("blankMod/output", "output/output");
	LOG(LogLevel::Info) << "<- Moving Mod Template";
	Utils::renameFolder("output/output", "output/" + theConfiguration.getOutputName());
	LOG(LogLevel::Info) << "<- Crafting .mod File";
	createModFile();

	// Record converter version
	LOG(LogLevel::Info) << "<- Writing version";
	outputVersion(versionParser);

	// Update bookmark starting dates
	LOG(LogLevel::Info) << "<- Updating bookmarks";
	modifyDefines();
	

	// Create common\countries path.
	std::string countriesPath = "output/" + theConfiguration.getOutputName() + "/common/countries";
	if (!Utils::TryCreateFolder(countriesPath))
	{
		return;
	}

	// Output common\countries.txt
	LOG(LogLevel::Info) << "<- Creating countries.txt";
	outputCommonCountries();

	// Create flags for all new countries.
	LOG(LogLevel::Info) << "-> Creating Flags";
	Flags flags;
	LOG(LogLevel::Info) << "-> Setting Flags";
	flags.setV2Tags(countries, countryMapper);
	LOG(LogLevel::Info) << "<- Writing Flags";
	flags.output();

	// Create localisations for all new countries. We don't actually know the names yet so we just use the tags as the names.
	LOG(LogLevel::Info) << "<- Writing Localisation Text";
	outputLocalisation();

	LOG(LogLevel::Info) << "<- Writing Provinces";
	outputProvinces();

	LOG(LogLevel::Info) << "<- Writing Countries";
	outputCountries();

	LOG(LogLevel::Info) << "<- Writing Diplomacy";
	diplomacy.output();

	LOG(LogLevel::Info) << "<- Writing Pops";
	outputPops();

	// verify countries got written
	LOG(LogLevel::Info) << "-> Verifying All Countries Written";
	verifyCountriesWritten();
}

void V2::World::outputVersion(const mappers::VersionParser& versionParser) const
{
	std::ofstream output("output/" + theConfiguration.getOutputName() + "/eu4tov2_version.txt");
	if (!output.is_open()) throw std::runtime_error("Error writing version file! Is the output folder writable?");
	output << versionParser;
	output.close();
}

void V2::World::outputCommonCountries() const
{
	std::ofstream output("output/" + theConfiguration.getOutputName() + "/common/countries.txt");
	if (!output.is_open()) throw std::runtime_error("Could not create countries file!");

	for (const auto& country: countries)
	{		
		const auto& dynamic = dynamicCountries.find(country.first);
		// First output all regular countries, order matters!
		if (dynamic == dynamicCountries.end())
		{
			output << country.first << " = \"countries/" << country.second->getCommonCountryFile() << "\"\n";
		}
	}
	output << "\n";
	output << "##HoD Dominions\n";
	output << "dynamic_tags = yes # any tags after this is considered dynamic dominions\n";	
	for (const auto& country: dynamicCountries)
	{
		output << country.first << " = \"countries/" << country.second->getCommonCountryFile() << "\"\n";
	}
	output.close();
}

void V2::World::outputLocalisation() const
{
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
		std::ofstream output("test.txt", std::ofstream::out | std::ofstream::trunc);
		output.close();
	}

	LOG(LogLevel::Info) << "<- Writing Localisation Names";
	std::ofstream output(localisationPath + "/0_Names.csv", std::ofstream::app);
	if (!output.is_open()) throw(std::runtime_error("Could not update localisation text file"));

	Utils::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/history/countries");
	Utils::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/history/units");
	for (auto country : countries)
	{
		if (country.second->isNewCountry())
		{
			output << country.second->getLocalisation();
		}
	}
	output.close();
}

void V2::World::outputProvinces() const
{
	Utils::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/history/provinces");
	for (auto province : provinces)
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
}

void V2::World::outputCountries() const
{
	for (const auto& country : countries)
	{
		// Country file
		if (!country.second->isDynamicCountry())
		{
			std::ofstream output("output/" + theConfiguration.getOutputName() + "/history/countries/" + country.second->getFilename());
			if (!output.is_open()) throw std::runtime_error("Could not create country history file " + country.second->getFilename());
			output << *country.second;
			output.close();
		}
		// commons file
		if (country.second->isDynamicCountry() || country.second->isNewCountry())
		{
			std::ofstream output("output/" + theConfiguration.getOutputName() + "/common/countries/" + country.second->getCommonCountryFile());
			if (!output.is_open()) throw std::runtime_error("Could not open output/" + theConfiguration.getOutputName() + "/common/countries/" + country.second->getCommonCountryFile());
			country.second->outputCommons(output);
			output.close();
		}
		// OOB
		std::ofstream output("output/" + theConfiguration.getOutputName() + "/history/units/" + country.first + "_OOB.txt");
		if (!output.is_open()) throw std::runtime_error("Could not create OOB file " + country.first + "_OOB.txt");
		country.second->outputOOB(output);
	}
}

void V2::World::modifyDefines() const
{
	auto potentialGPs = countCivilizedNations();
	std::string startDate = "<STARTDATE>";
	std::string numGPs = "GREAT_NATIONS_COUNT = 8";
	std::ostringstream incomingDefines, incomingBookmarks;

	// Edit starting date in defines + adjust GP count if needed
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

	// Edit bookmark start
	std::ifstream bookmarks_txt("output/" + theConfiguration.getOutputName() + "/common/bookmarks.txt");
	incomingBookmarks << bookmarks_txt.rdbuf();
	bookmarks_txt.close();
	std::string strBookmarks = incomingBookmarks.str();
	size_t pos2 = strBookmarks.find(startDate);
	strBookmarks.replace(pos2, startDate.length(), theConfiguration.getLastEU4Date().toString());
	std::ofstream out_bookmarks_txt("output/" + theConfiguration.getOutputName() + "/common/bookmarks.txt");
	out_bookmarks_txt << strBookmarks;
	out_bookmarks_txt.close();
}

void V2::World::verifyCountriesWritten() const
{
	std::ifstream V2CountriesInput;
	V2CountriesInput.open(("output/" + theConfiguration.getOutputName() + "/common/countries.txt").c_str());
	if (!V2CountriesInput.is_open()) throw std::runtime_error("Could not open countries.txt");

	while (!V2CountriesInput.eof())
	{
		std::string line;
		getline(V2CountriesInput, line);

		if (line[0] == '#' || line.size() < 3) continue;
		if (line.substr(0, 12) == "dynamic_tags") continue;

		std::string countryFileName;
		const unsigned int start = line.find_first_of('/');
		const unsigned int size = line.find_last_of('\"') - start - 1;
		countryFileName = line.substr(start + 1, size);

		if (Utils::DoesFileExist("output/" + theConfiguration.getOutputName() + "/common/countries/" + countryFileName)) { continue; }
		if (Utils::DoesFileExist(theConfiguration.getVic2Path() + "/common/countries/" + countryFileName)) { continue; }
		LOG(LogLevel::Warning) << "common/countries/" << countryFileName << " does not exists. This will likely crash Victoria 2.";
	}
	V2CountriesInput.close();
}

void V2::World::createModFile() const
{
	std::ofstream output("output/" + theConfiguration.getOutputName() + ".mod");
	if (!output.is_open()) throw std::runtime_error("Could not create " + theConfiguration.getOutputName() + ".mod");
	LOG(LogLevel::Info) << "\t-> Writing to: " << "output/" + theConfiguration.getOutputName() + ".mod";
	output << modFile;
	output.close();
}

void V2::World::outputPops() const
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

std::shared_ptr<V2::Province> V2::World::getProvince(const int provID) const
{
	const auto& provinceItr = provinces.find(provID);
	return provinceItr != provinces.end() ? provinceItr->second : nullptr;
}

std::shared_ptr<V2::Country> V2::World::getCountry(const std::string& tag) const
{
	const auto& countryItr = countries.find(tag);
	return (countryItr != countries.end()) ? countryItr->second : nullptr;
}
