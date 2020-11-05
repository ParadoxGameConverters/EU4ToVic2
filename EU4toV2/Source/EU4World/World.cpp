#include "World.h"
#include "../Configuration.h"
#include "../Helpers/rakaly_wrapper.h"
#include "Country/Countries.h"
#include "Country/EU4Country.h"
#include "GameVersion.h"
#include "Localisation/EU4Localisation.h"
#include "Log.h"
#include "Mods/Mods.h"
#include "NationMerger/NationMergeParser.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include "Provinces/EU4Province.h"
#include "Regions/Areas.h"
#include "Regions/SuperRegions.h"
#include "Relations/EU4Empire.h"
#include <ZipFile.h>
#include <algorithm>
#include <exception>
#include <filesystem>
#include <fstream>
#include <set>
#include <string>
namespace fs = std::filesystem;

EU4::World::World(const mappers::IdeaEffectMapper& ideaEffectMapper)
{
	LOG(LogLevel::Info) << "*** Hello EU4, loading World. ***";
	registerKeyword("EU4txt", [](const std::string& unused, std::istream& theStream) {
	});
	registerKeyword("date", [](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString dateString(theStream);
		theConfiguration.setLastEU4Date(date(dateString.getString()));
	});
	registerKeyword("start_date", [](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString startDateString(theStream);
		theConfiguration.setStartEU4Date(date(startDateString.getString()));
	});
	registerRegex("(multiplayer_)?random_seed", [](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString randomSeed(theStream);
		auto theSeed = randomSeed.getString();
		if (theSeed.size() > 5)
			theSeed = theSeed.substr(theSeed.size() - 5);
		try
		{
			theConfiguration.setEU4RandomSeed(std::stoi(theSeed));
		}
		catch (std::exception& e)
		{
			Log(LogLevel::Error) << "Failed reading random_seed, setting 0: " << e.what();
			theConfiguration.setEU4RandomSeed(0);
		}
	});
	registerKeyword("savegame_version", [this](const std::string& unused, std::istream& theStream) {
		version = std::make_unique<GameVersion>(theStream);
		theConfiguration.setEU4Version(*version);
		Log(LogLevel::Info) << "Savegave version: " << *version;
	});
	registerKeyword("dlc_enabled", [](const std::string& unused, std::istream& theStream) {
		const commonItems::stringList theDLCs(theStream);
		theConfiguration.setActiveDLCs(theDLCs.getStrings());
	});
	registerKeyword("mod_enabled", [](const std::string& unused, std::istream& theStream) {
		const commonItems::stringList modList(theStream);
		Mods theMods(modList.getStrings());
	});
	registerKeyword("revolution_target", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString revTargetStr(theStream);
		revolutionTargetString = revTargetStr.getString();
	});
	registerKeyword("celestial_empire", [this](const std::string& unused, std::istream& theStream) {
		const EU4Empire empireBlock(theStream);
		celestialEmperor = empireBlock.getEmperor();
	});
	registerKeyword("empire", [this](const std::string& unused, std::istream& theStream) {
		const EU4Empire empireBlock(theStream);
		holyRomanEmperor = empireBlock.getEmperor();
	});
	registerKeyword("emperor", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString emperorStr(theStream);
		holyRomanEmperor = emperorStr.getString();
	});
	registerKeyword("provinces", [this](const std::string& unused, std::istream& theStream) {
		LOG(LogLevel::Info) << "-> Loading Provinces";
		provinces = std::make_unique<Provinces>(theStream);

		const auto& possibleDate = provinces->getProvince(1)->getFirstOwnedDate();
		if (possibleDate)
			theConfiguration.setFirstEU4Date(*possibleDate);
	});
	registerKeyword("countries", [this, ideaEffectMapper](const std::string& unused, std::istream& theStream) {
		LOG(LogLevel::Info) << "-> Loading Countries";
		const Countries processedCountries(*version, theStream, ideaEffectMapper);
		auto theProcessedCountries = processedCountries.getTheCountries();
		theCountries.swap(theProcessedCountries);
	});
	registerKeyword("diplomacy", [this](const std::string& unused, std::istream& theStream) {
		LOG(LogLevel::Info) << "-> Loading Diplomacy";
		const EU4Diplomacy theDiplomacy(theStream);
		diplomacy = theDiplomacy.getAgreements();
		LOG(LogLevel::Info) << "-> Loaded " << diplomacy.size() << " agreements";
	});
	registerKeyword("map_area_data", [](const std::string& unused, std::istream& theStream) {
		LOG(LogLevel::Info) << "-> Loading Map Area Data";
		commonItems::ignoreItem(unused, theStream);
		LOG(LogLevel::Info) << "XX Promptly Ignoring Map Area Data.";
	});
	registerKeyword("active_war", [this](const std::string& unused, std::istream& theStream) {
		const War newWar(theStream);
		wars.push_back(newWar);
	});
	registerKeyword("change_price", [this](const std::string& unused, std::istream& theStream) {
		const TradeGoods theGoods(theStream);
		tradeGoods = theGoods;
	});

	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	superGroupMapper.init();
	Log(LogLevel::Progress) << "6 %";

	LOG(LogLevel::Info) << "-> Verifying EU4 save.";
	verifySave();
	Log(LogLevel::Progress) << "7 %";

	LOG(LogLevel::Info) << "-> Importing EU4 save.";
	if (!saveGame.compressed)
	{
		std::ifstream inBinary(fs::u8path(theConfiguration.getEU4SaveGamePath()), std::ios::binary);
		if (!inBinary.is_open())
		{
			LOG(LogLevel::Error) << "Could not open " << theConfiguration.getEU4SaveGamePath() << " for parsing.";
			throw std::runtime_error("Could not open " + theConfiguration.getEU4SaveGamePath() + " for parsing.");
		}
		std::stringstream inStream;
		inStream << inBinary.rdbuf();
		saveGame.gamestate = inStream.str();
	}
	Log(LogLevel::Progress) << "8 %";

	verifySaveContents();
	Log(LogLevel::Progress) << "9 %";

	auto metaData = std::istringstream(saveGame.metadata);
	auto gameState = std::istringstream(saveGame.gamestate);
	parseStream(metaData);
	parseStream(gameState);
	Log(LogLevel::Progress) << "15 %";

	clearRegisteredKeywords();
	// With mods loaded we can init stuff that requires them.
	modifierTypes = std::make_unique<Modifiers>();
	buildingTypes = std::make_unique<mappers::Buildings>();
	unitTypeMapper.initUnitTypeMapper();
	cultureGroupsMapper.initForEU4();
	Log(LogLevel::Progress) << "16 %";

	LOG(LogLevel::Info) << "*** Building world ***";
	LOG(LogLevel::Info) << "-> Loading Empires";
	setEmpires();
	Log(LogLevel::Progress) << "17 %";

	LOG(LogLevel::Info) << "-> Setting Province Weight";
	addTradeGoodsToProvinces();
	Log(LogLevel::Progress) << "18 %";

	LOG(LogLevel::Info) << "-> Processing Province Info";
	addProvinceInfoToCountries();
	Log(LogLevel::Progress) << "19 %";

	LOG(LogLevel::Info) << "-> Determining Province Weights";
	provinces->determineTotalProvinceWeights(theConfiguration);
	Log(LogLevel::Progress) << "20 %";

	LOG(LogLevel::Info) << "-> Loading Regions";
	loadRegions();
	Log(LogLevel::Progress) << "21 %";

	LOG(LogLevel::Info) << "-> Determining Demographics";
	buildPopRatios();
	Log(LogLevel::Progress) << "22 %";

	LOG(LogLevel::Info) << "-> Cataloguing Native Fauna";
	catalogueNativeCultures();
	Log(LogLevel::Progress) << "24 %";

	LOG(LogLevel::Info) << "-> Clasifying Invasive Fauna";
	generateNeoCultures();
	Log(LogLevel::Progress) << "25 %";

	LOG(LogLevel::Info) << "-> Reading Countries";
	readCommonCountries();
	Log(LogLevel::Progress) << "26 %";

	LOG(LogLevel::Info) << "-> Setting Localizations";
	setLocalisations();
	Log(LogLevel::Progress) << "27 %";

	LOG(LogLevel::Info) << "-> Resolving Regiments";
	resolveRegimentTypes();
	Log(LogLevel::Progress) << "28 %";

	LOG(LogLevel::Info) << "-> Merging Nations";
	mergeNations();
	Log(LogLevel::Progress) << "29 %";

	LOG(LogLevel::Info) << "-> Calculating Industry";
	calculateIndustry();
	Log(LogLevel::Progress) << "30 %";

	LOG(LogLevel::Info) << "-> Viva la revolution!";
	loadRevolutionTarget();
	if (!revolutionTargetString.empty())
	{
		LOG(LogLevel::Info) << " ^^^ Revolution Lives!";
	}
	else
	{
		LOG(LogLevel::Info) << " vvv ... revolution failed. :/";
	}
	Log(LogLevel::Progress) << "31 %";

	LOG(LogLevel::Info) << "-> Doing Accounting and dishes";
	fillHistoricalData();
	Log(LogLevel::Progress) << "32 %";

	LOG(LogLevel::Info) << "-> Dropping Empty Nations";
	removeEmptyNations();
	if (theConfiguration.getRemoveType() == Configuration::DEADCORES::DeadCores)
	{
		removeDeadLandlessNations();
	}
	else if (theConfiguration.getRemoveType() == Configuration::DEADCORES::AllCores)
	{
		removeLandlessNations();
	}
	LOG(LogLevel::Info) << "*** Good-bye EU4, you served us well. ***";
	Log(LogLevel::Progress) << "40 %";
}

void EU4::World::calculateIndustry() const
{
	for (const auto& country: theCountries)
	{
		country.second->buildManufactoryCount(theCountries);
	}
}

void EU4::World::buildPopRatios() const
{
	for (const auto& province: provinces->getAllProvinces())
	{
		province.second->buildPopRatio(superGroupMapper, *regions);
	}
}

void EU4::World::generateNeoCultures()
{
	for (const auto& province: provinces->getAllProvinces())
	{
		for (const auto& popratio: province.second->getPopRatios())
		{
			// Are we operating within native super region for this culture's pop ratio?
			const auto& superRegionName = regions->getParentSuperRegionName(province.first);
			if (!superRegionName)
				continue;
			const auto& currentCulture = popratio.getCulture();
			if (nativeCultures[*superRegionName].count(currentCulture))
				continue;

			// Are we a neoculture? Bail if so.
			if (!popratio.getOriginalCulture().empty())
				continue;

			// Are we within the supergroup? Find out where that culture is native.
			std::string nativeSuperRegionName;
			for (const auto& nativeRegion: nativeCultures)
				if (nativeRegion.second.count(currentCulture))
					nativeSuperRegionName = nativeRegion.first;
			if (nativeSuperRegionName.empty())
			{
				// This is not unusual. Oromo appear later and are not relevant to us.
				// For eu4's american, mexican, brazilian and similar, we'll merge them later with our neocultures through culture_maps.txt.
				continue;
			}
			const auto& currentSuperGroup = superGroupMapper.getGroupForSuperRegion(*superRegionName);
			if (!currentSuperGroup)
			{
				Log(LogLevel::Warning) << "Super-Region " << *superRegionName << " has no defined super-group in worlds_supergroups.txt! Fix this!";
				continue;
			}
			const auto& nativeSuperGroup = superGroupMapper.getGroupForSuperRegion(nativeSuperRegionName);
			if (!nativeSuperGroup)
			{
				Log(LogLevel::Warning) << "Super-Region " << nativeSuperRegionName << " has no defined super-group in worlds_supergroups.txt! Fix this!";
				continue;
			}
			if (*nativeSuperGroup == *currentSuperGroup)
				continue; // Do not mutate within the same super group.

			// Check global cache if we already did this pair.
			std::string neoCulture;
			const auto& genItr = generatedCultures.find(std::make_pair(currentCulture, *superRegionName));
			if (genItr == generatedCultures.end())
			{
				// We need to roll sleeves and get to work.
				neoCulture = generateNeoCulture(*superRegionName, currentCulture);
				generatedCultures.insert(std::make_pair(std::make_pair(currentCulture, *superRegionName), neoCulture));
			}
			else
			{
				neoCulture = genItr->second;
			}
			// Now update the pop ratio.
			province.second->updatePopRatioCulture(currentCulture, neoCulture, *superRegionName);
		}
	}
}

std::string EU4::World::generateNeoCulture(const std::string& superRegionName, const std::string& oldCultureName)
{
	// pull culture group name
	const auto& cultureGroupOpt = cultureGroupsMapper.getGroupForCulture(oldCultureName);
	if (!cultureGroupOpt)
	{
		// Bail gracefully.
		Log(LogLevel::Warning) << "Culture " << oldCultureName << " has no culture group defined! This should not happen!";
		return oldCultureName;
	}

	// This is the new culture name.
	const auto neoCultureName = cultureGroupOpt->getName() + "_" + superRegionName + "_culture";

	// Grab culture definitions.
	const auto& cultureItr = cultureGroupOpt->getCultures().find(oldCultureName);
	if (cultureItr == cultureGroupOpt->getCultures().end())
	{
		// what is going in in there?
		Log(LogLevel::Warning) << "Culture " << oldCultureName << " has no culture definitions! This should not happen!";
		return oldCultureName;
	}
	auto neoCulture = cultureItr->second;

	// Ditch cultureGroupOpt and grab the editable version.
	auto cultureGroup = cultureGroupsMapper.retrieveCultureGroup(oldCultureName);

	// We may already have this neoCulture registered (generated by another culture within the same group).
	const auto& neoCultureItr = cultureGroupOpt->getCultures().find(neoCultureName);
	if (neoCultureItr == cultureGroupOpt->getCultures().end())
	{
		// We're golden. Register neoCulture.
		cultureGroup->addNeoCulture(neoCultureName, neoCulture, oldCultureName);
	}
	else
	{
		// We need to append this culture's names on top the existing ones.
		cultureGroup->mergeCulture(neoCultureName, neoCulture);
	}

	return neoCultureName;
}

void EU4::World::catalogueNativeCultures()
{
	for (const auto& province: provinces->getAllProvinces())
	{
		if (province.second->getOriginalCulture().empty())
			continue;
		const auto& superRegionName = regions->getParentSuperRegionName(province.first);
		if (superRegionName)
			nativeCultures[*superRegionName].insert(province.second->getOriginalCulture());
	}
}

void EU4::World::fillHistoricalData()
{
	for (const auto& country: theCountries)
		historicalData.emplace_back(std::make_pair(country.first, country.second->getHistoricalEntry()));
}

void EU4::World::verifySaveContents()
{
	if (saveGame.gamestate[0] == 'E' && saveGame.gamestate[1] == 'U' && saveGame.gamestate[2] == '4' && saveGame.gamestate[3] == 'b' &&
		 saveGame.gamestate[4] == 'i' && saveGame.gamestate[5] == 'n')
	{
		saveGame.gamestate = rakaly::meltEU4(saveGame.gamestate);
		saveGame.metadata = rakaly::meltEU4(saveGame.metadata);
	}
}

void EU4::World::verifySave()
{
	std::ifstream saveFile(fs::u8path(theConfiguration.getEU4SaveGamePath()));
	if (!saveFile.is_open())
		throw std::runtime_error("Could not open save! Exiting!");

	char buffer[3];
	saveFile.get(buffer, 3);
	if (buffer[0] == 'P' && buffer[1] == 'K')
	{
		if (!uncompressSave())
			throw std::runtime_error("Failed to unpack the compressed save!");
	}
	saveFile.close();
}

bool EU4::World::uncompressSave()
{
	auto savefile = ZipFile::Open(theConfiguration.getEU4SaveGamePath());
	if (!savefile)
		return false;
	for (size_t entryNum = 0; entryNum < savefile->GetEntriesCount(); ++entryNum)
	{
		const auto& entry = savefile->GetEntry(static_cast<int>(entryNum));
		const auto& name = entry->GetName();
		if (name == "meta")
		{
			LOG(LogLevel::Info) << ">> Uncompressing metadata";
			saveGame.metadata = std::string{std::istreambuf_iterator<char>(*entry->GetDecompressionStream()), std::istreambuf_iterator<char>()};
		}
		else if (name == "gamestate")
		{
			LOG(LogLevel::Info) << ">> Uncompressing gamestate";
			saveGame.gamestate = std::string{std::istreambuf_iterator<char>(*entry->GetDecompressionStream()), std::istreambuf_iterator<char>()};
		}
		else if (name == "ai")
		{
			LOG(LogLevel::Info) << ">> Uncompressing ai and forgetting it existed";
			saveGame.compressed = true;
		}
		else
			throw std::runtime_error("Unrecognized savegame structure!");
	}
	return true;
}


void EU4::World::loadRevolutionTarget()
{
	if (!revolutionTargetString.empty())
	{
		auto country = theCountries.find(revolutionTargetString);
		if (country != theCountries.end())
		{
			country->second->viveLaRevolution(true);
		}
	}
}

void EU4::World::addTradeGoodsToProvinces() const
{
	for (auto& province: provinces->getAllProvinces())
	{
		const auto& price = tradeGoods.getPrice(province.second->getTradeGoods());
		if (!price)
		{
			Log(LogLevel::Warning) << "Unknown trade good in province " << province.first << " - " << province.second->getName();
		}
		else
		{
			province.second->setTradeGoodPrice(*price);
		}
		province.second->determineProvinceWeight(*buildingTypes, *modifierTypes);
	}
}

void EU4::World::addProvinceInfoToCountries()
{
	// add province owner info to countries
	for (auto& province: provinces->getAllProvinces())
	{
		auto owner = theCountries.find(province.second->getOwnerString());
		if (owner != theCountries.end())
		{
			owner->second->addProvince(province.second);
		}
	}

	// add province core info to countries
	for (const auto& province: provinces->getAllProvinces())
	{
		auto cores = province.second->getCores();
		for (const auto& core: cores)
		{
			const auto& country = theCountries.find(core);
			if (country != theCountries.end())
			{
				country->second->addCore(province.second);
			}
		}
	}
}

void EU4::World::loadRegions()
{
	if (*version >= GameVersion("1.14"))
	{
		loadEU4RegionsNewVersion();
	}
	else
	{
		loadEU4RegionsOldVersion();
	}
}

void EU4::World::assignProvincesToAreas(const std::map<std::string, std::set<int>>& theAreas) const
{
	for (const auto& area: theAreas)
	{
		const auto& areaName = area.first;
		for (auto provNum: area.second)
		{
			try
			{
				provinces->getProvince(provNum)->setArea(areaName);
			}
			catch (std::exception& e)
			{
				LOG(LogLevel::Warning) << e.what();
			}
		}
	}
}

void EU4::World::loadEU4RegionsOldVersion()
{
	auto regionFilename = theConfiguration.getEU4Path() + "/map/region.txt";

	for (const auto& itr: theConfiguration.getEU4Mods())
	{
		if (!commonItems::DoesFileExist(itr + "/map/region.txt"))
		{
			continue;
		}

		regionFilename = itr + "/map/region.txt";
	}

	std::ifstream theStream(fs::u8path(regionFilename));
	Areas installedAreas(theStream);
	theStream.close();
	assignProvincesToAreas(installedAreas.getAreas());

	regions = std::make_unique<Regions>(installedAreas);
}


void EU4::World::loadEU4RegionsNewVersion()
{
	auto areaFilename = theConfiguration.getEU4Path() + "/map/area.txt";
	auto regionFilename = theConfiguration.getEU4Path() + "/map/region.txt";
	auto superRegionFilename = theConfiguration.getEU4Path() + "/map/superregion.txt";
	for (const auto& itr: theConfiguration.getEU4Mods())
	{
		if (!commonItems::DoesFileExist(itr + "/map/area.txt"))
			continue;
		areaFilename = itr + "/map/area.txt";
	}
	for (const auto& itr: theConfiguration.getEU4Mods())
	{
		if (!commonItems::DoesFileExist(itr + "/map/region.txt"))
			continue;
		regionFilename = itr + "/map/region.txt";
	}
	for (const auto& itr: theConfiguration.getEU4Mods())
	{
		if (!commonItems::DoesFileExist(itr + "/map/superregion.txt"))
			continue;
		superRegionFilename = itr + "/map/superregion.txt";
	}

	std::ifstream areaStream(fs::u8path(areaFilename));
	if (!areaStream.is_open())
		throw std::runtime_error("Could not open map/area.txt!");
	Areas installedAreas(areaStream);
	areaStream.close();
	assignProvincesToAreas(installedAreas.getAreas());

	std::ifstream superRegionFile(fs::u8path(superRegionFilename));
	if (!superRegionFile.is_open())
		throw std::runtime_error("Could not open map/superregion.txt!");
	SuperRegions sRegions(superRegionFile);
	superRegionFile.close();

	std::ifstream regionStream(fs::u8path(regionFilename));
	if (!regionStream.is_open())
		throw std::runtime_error("Could not open map/region.txt!");
	regions = std::make_unique<Regions>(sRegions, installedAreas, regionStream);
	regionStream.close();
}


void EU4::World::readCommonCountries()
{
	std::ifstream commonCountries(fs::u8path(theConfiguration.getEU4Path() + "/common/country_tags/00_countries.txt")); // the data in the countries file
	if (!commonCountries.is_open())
		throw std::runtime_error("Could not open common/country_tags/00_countries.txt!");
	readCommonCountriesFile(commonCountries, theConfiguration.getEU4Path());
	for (const auto& itr: theConfiguration.getEU4Mods())
	{
		auto fileNames = commonItems::GetAllFilesInFolder(itr + "/common/country_tags/");
		for (const auto& fileItr: fileNames)
		{
			std::ifstream convertedCommonCountries(fs::u8path(itr + "/common/country_tags/" + fileItr)); // a stream of the data in the converted countries file
			if (!convertedCommonCountries.is_open())
				throw std::runtime_error("Could not open common/country_tags/" + fileItr + "!");
			readCommonCountriesFile(convertedCommonCountries, itr);
		}
	}
}

void EU4::World::readCommonCountriesFile(std::istream& in, const std::string& rootPath)
{
	// Add any info from common\countries
	const int maxLineLength = 10000; // the maximum line length
	char line[maxLineLength];			// the line being processed

	while (true)
	{
		in.getline(line, maxLineLength);
		if (in.eof())
		{
			return;
		}
		std::string countryLine = line;
		if (countryLine.size() >= 6 && countryLine[0] != '#')
		{
			// First three characters must be the tag.
			auto tag = countryLine.substr(0, 3);
			if (auto findIter = theCountries.find(tag); findIter != theCountries.end())
			{
				auto country = findIter->second;

				// The country file name is all the text after the equals sign (possibly in quotes).
				const auto commentPos = countryLine.find('#', 3);
				if (commentPos != std::string::npos)
				{
					countryLine = countryLine.substr(0, commentPos);
				}
				const auto equalPos = countryLine.find('=', 3);
				const auto beginPos = countryLine.find_first_not_of(' ', equalPos + 1);
				const auto endPos = countryLine.find_last_of('\"') + 1;
				auto fileName = countryLine.substr(beginPos, endPos - beginPos);
				if (fileName.front() == '"' && fileName.back() == '"')
				{
					fileName = fileName.substr(1, fileName.size() - 2);
				}
				std::replace(fileName.begin(), fileName.end(), '/', '/');

				// Parse the country file.
				auto fullFilename = rootPath + "/common/" + fileName;
				const auto lastPathSeparatorPos = fullFilename.find_last_of('/');
				auto localFileName = fullFilename.substr(lastPathSeparatorPos + 1, std::string::npos);
				if (commonItems::DoesFileExist(fullFilename))
				{
					country->readFromCommonCountry(localFileName, fullFilename);
				}
			}
		}
	}
}

void EU4::World::setLocalisations()
{
	const EU4Localisation localisation;

	for (const auto& theCountry: theCountries)
	{
		const auto& nameLocalisations = localisation.getTextInEachLanguage(theCountry.second->getTag());
		if (nameLocalisations)
			for (const auto& [language, name]: *nameLocalisations)
				theCountry.second->setLocalisationName(language, name);

		const auto& adjectiveLocalisations = localisation.getTextInEachLanguage(theCountry.second->getTag() + "_ADJ");
		if (adjectiveLocalisations)
			for (const auto& [language, adjective]: *adjectiveLocalisations)
				theCountry.second->setLocalisationAdjective(language, adjective);
	}
}

void EU4::World::resolveRegimentTypes()
{
	for (const auto& country: theCountries)
	{
		country.second->resolveRegimentTypes(unitTypeMapper);
	}
}

void EU4::World::mergeNations()
{
	const NationMergeParser mergeParser;

	if (mergeParser.getMergeDaimyos())
		uniteJapan();

	for (const auto& mergeBlock: mergeParser.getMergeBlocks())
	{
		if (mergeBlock.getMerge() && !mergeBlock.getMaster().empty())
		{
			LOG(LogLevel::Info) << "- Merging nations for: " << mergeBlock.getMaster();
			auto master = getCountry(mergeBlock.getMaster());
			for (const auto& slaveTag: mergeBlock.getSlaves())
			{
				const auto& slave = getCountry(slaveTag);
				master->eatCountry(slave);
			}
		}
	}
}

void EU4::World::uniteJapan()
{
	LOG(LogLevel::Info) << "-> Uniting Japan";

	std::shared_ptr<Country> japan;

	if (*version >= GameVersion("1.20.0.0"))
	{
		for (const auto& country: theCountries)
		{
			if (country.second->getPossibleShogun())
			{
				const auto& tag = country.first;
				LOG(LogLevel::Info) << "- " << tag << " is the shogun.";
				japan = getCountry(tag);
			}
		}
	}
	else
	{
		japan = getCountry("JAP");
	}
	if (japan == nullptr)
		return;
	if (japan->hasFlag("united_daimyos_of_japan"))
		return;

	for (const auto& country: theCountries)
	{
		if (country.second->getPossibleDaimyo())
		{
			japan->eatCountry(country.second);
		}
	}
}

void EU4::World::removeEmptyNations()
{
	std::map<std::string, std::shared_ptr<Country>> survivingCountries;

	for (const auto& country: theCountries)
	{
		auto countryProvinces = country.second->getProvinces();
		auto countryCores = country.second->getCores();
		if (!countryProvinces.empty() || !countryCores.empty())
		{
			survivingCountries.insert(country);
		}
	}

	theCountries.swap(survivingCountries);
}

void EU4::World::removeDeadLandlessNations()
{
	std::map<std::string, std::shared_ptr<Country>> landlessCountries;
	for (const auto& country: theCountries)
	{
		auto countryProvinces = country.second->getProvinces();
		if (countryProvinces.empty())
		{
			landlessCountries.insert(country);
		}
	}

	for (const auto& country: landlessCountries)
	{
		if (!country.second->cultureSurvivesInCores(theCountries))
		{
			theCountries.erase(country.first);
		}
	}
}

void EU4::World::removeLandlessNations()
{
	std::map<std::string, std::shared_ptr<Country>> survivingCountries;

	for (const auto& country: theCountries)
	{
		auto provinces = country.second->getProvinces();
		if (provinces.empty())
		{
			LOG(LogLevel::Debug) << "Removing landless nation " << country.first;
		}
		else
		{
			survivingCountries.insert(country);
		}
	}

	theCountries.swap(survivingCountries);
}

void EU4::World::setEmpires()
{
	for (const auto& country: theCountries)
	{
		// set HRE stuff
		if (country.second->getCapital() != 0 && provinces->getProvince(country.second->getCapital())->inHre())
		{
			country.second->setInHRE(true);
		}
		if (country.second->getTag() == holyRomanEmperor)
		{
			country.second->setEmperor(true);
		}
		if (country.second->getTag() == celestialEmperor)
		{
			country.second->setCelestialEmperor(true);
		}
	}
}

std::shared_ptr<EU4::Country> EU4::World::getCountry(const std::string& tag) const
{
	const auto& country = theCountries.find(tag);
	if (country != theCountries.end())
		return country->second;
	return nullptr;
}

std::shared_ptr<EU4::Province> EU4::World::getProvince(int provNum) const
{
	return provinces->getProvince(provNum);
}

bool EU4::World::isRandomWorld() const
{
	auto isRandomWorld = true;

	for (const auto& sourceCountry: theCountries)
	{
		if (sourceCountry.first[0] != 'D' && sourceCountry.second->getRandomName().empty())
		{
			isRandomWorld = false;
		}
	}
	return isRandomWorld;
}
