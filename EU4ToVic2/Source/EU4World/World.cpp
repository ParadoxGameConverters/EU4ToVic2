#include "World.h"
#include "AgreementMapper/AgreementMapper.h"
#include "CommonFunctions.h"
#include "CommonRegexes.h"
#include "Configuration.h"
#include "Country/Countries.h"
#include "Country/EU4Country.h"
#include "CultureGroups/CultureGroup.h"
#include "GameVersion.h"
#include "Localization/EU4Localization.h"
#include "Log.h"
#include "Mods/ModNames.h"
#include "NationMerger/NationMergeParser.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include "Provinces/EU4Province.h"
#include "Regions/Areas.h"
#include "Regions/SuperRegions.h"
#include "Relations/EU4Empire.h"
#include "StringUtils.h"
#include "rakaly.h"
#include "zip.h"
#include <exception>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <set>
#include <string>
namespace fs = std::filesystem;

EU4::World::World(const mappers::IdeaEffectMapper& ideaEffectMapper, const commonItems::ConverterVersion& converterVersion)
{
	Log(LogLevel::Info) << "*** Hello EU4, loading World. ***";
	registerKeys(ideaEffectMapper, converterVersion);

	superGroupMapper.init();
	Log(LogLevel::Progress) << "6 %";

	Log(LogLevel::Info) << "-> Verifying EU4 save.";
	verifySave();
	Log(LogLevel::Progress) << "7 %";

	Log(LogLevel::Progress) << "\t* Importing Save. *";
	auto metaData = std::istringstream(saveGame.metadata);
	auto gameState = std::istringstream(saveGame.gamestate);
	parseStream(metaData);
	if (!saveGame.metadata.empty())
		saveGame.parsedMeta = true;
	parseStream(gameState);
	Log(LogLevel::Progress) << "\t* Import Complete. *";
	Log(LogLevel::Progress) << "15 %";

	clearRegisteredKeywords();
	// With mods loaded we can init stuff that requires them.
	cultureGroupsMapper = std::make_shared<mappers::CultureGroupsManager>();
	cultureGroupsMapper->initForEU4();
	buildingTypes = std::make_unique<mappers::Buildings>();
	unitTypeMapper.initUnitTypeMapper();
	if (theConfiguration.isVN())
		theConfiguration.setLastEU4Date(date("1836.1.1"));
	EU4localization.initializeFromEU4Installation();
	Log(LogLevel::Progress) << "16 %";

	Log(LogLevel::Info) << "*** Building world ***";
	Log(LogLevel::Info) << "-> Loading Empires";
	setEmpires();
	importShatteredHreTag();
	Log(LogLevel::Progress) << "17 %";

	Log(LogLevel::Info) << "-> Calculating Province Weights";
	buildProvinceWeights();
	Log(LogLevel::Progress) << "18 %";

	Log(LogLevel::Info) << "-> Processing Province Info";
	addProvinceInfoToCountries();
	Log(LogLevel::Progress) << "19 %";

	Log(LogLevel::Info) << "-> Loading Regions";
	loadRegions();
	Log(LogLevel::Progress) << "21 %";

	Log(LogLevel::Info) << "-> Determining Demographics";
	buildPopRatios();
	Log(LogLevel::Progress) << "22 %";

	Log(LogLevel::Info) << "-> Cataloguing Native Fauna";
	catalogueNativeCultures();
	Log(LogLevel::Progress) << "24 %";

	Log(LogLevel::Info) << "-> Clasifying Invasive Fauna";
	generateNeoCultures();
	Log(LogLevel::Progress) << "25 %";

	Log(LogLevel::Info) << "-> Reading Countries";
	readCommonCountries();
	Log(LogLevel::Progress) << "26 %";

	Log(LogLevel::Info) << "-> Setting Localizations";
	setLocalizations();
	Log(LogLevel::Progress) << "27 %";

	Log(LogLevel::Info) << "-> Resolving Regiments";
	resolveRegimentTypes();
	Log(LogLevel::Progress) << "28 %";

	Log(LogLevel::Info) << "-> Merging Nations";
	mergeNations();
	Log(LogLevel::Progress) << "29 %";

	Log(LogLevel::Info) << "-> Calculating Industry";
	calculateIndustry();
	Log(LogLevel::Progress) << "30 %";

	Log(LogLevel::Info) << "-> Viva la revolution!";
	loadRevolutionTarget();
	if (!revolutionTargetString.empty())
	{
		Log(LogLevel::Info) << " ^^^ Revolution Lives!";
	}
	else
	{
		Log(LogLevel::Info) << " vvv ... revolution failed. :/";
	}
	Log(LogLevel::Progress) << "31 %";

	Log(LogLevel::Info) << "-> Doing Accounting and dishes";
	fillHistoricalData();
	Log(LogLevel::Progress) << "32 %";

	Log(LogLevel::Info) << "-> Dropping Empty Nations";
	removeEmptyNations();
	if (theConfiguration.getRemoveType() == Configuration::DEADCORES::DeadCores)
	{
		removeDeadLandlessNations();
	}
	else if (theConfiguration.getRemoveType() == Configuration::DEADCORES::AllCores)
	{
		removeLandlessNations();
	}

	Log(LogLevel::Info) << "-> Marking new world countries";
	markNewWorldCountries();
	Log(LogLevel::Progress) << "33 %";

	Log(LogLevel::Info) << "*** Good-bye EU4, you served us well. ***";
	Log(LogLevel::Progress) << "40 %";
}

void EU4::World::registerKeys(const mappers::IdeaEffectMapper& ideaEffectMapper, const commonItems::ConverterVersion& converterVersion)
{
	revolutionParser.registerKeyword("revolution_target", [this](std::istream& theStream) {
		revolutionTargetString = commonItems::getString(theStream);
	});
	revolutionParser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	registerKeyword("EU4txt", [](std::istream& theStream) {
	});
	registerKeyword("date", [this](std::istream& theStream) {
		if (saveGame.parsedMeta)
			commonItems::ignoreItem("unused", theStream);
		else
			theConfiguration.setLastEU4Date(date(commonItems::getString(theStream)));
	});
	registerKeyword("start_date", [](std::istream& theStream) {
		theConfiguration.setStartEU4Date(date(commonItems::getString(theStream)));
	});
	registerRegex("(multiplayer_)?random_seed", [](const std::string& unused, std::istream& theStream) {
		auto theSeed = commonItems::getString(theStream);
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
	registerKeyword("savegame_version", [this, converterVersion](std::istream& theStream) {
		if (saveGame.parsedMeta)
		{
			commonItems::ignoreItem("unused", theStream);
		}
		else
		{
			version = std::make_unique<GameVersion>(theStream);
			theConfiguration.setEU4Version(*version);
			Log(LogLevel::Info) << "Savegave version: " << *version;
			if (theConfiguration.isHpmEnabled() && *version < GameVersion("1.31"))
				throw std::runtime_error("HPM hybridization can only be used on 1.31 saves or higher.");
			if (converterVersion.getMinSource() > *version)
			{
				Log(LogLevel::Error) << "Converter requires a minimum save from v" << converterVersion.getMinSource().toShortString();
				throw std::runtime_error("Savegame vs converter version mismatch!");
			}
			if (!converterVersion.getMaxSource().isLargerishThan(*version))
			{
				Log(LogLevel::Error) << "Converter requires a maximum save from v" << converterVersion.getMaxSource().toShortString();
				throw std::runtime_error("Savegame vs converter version mismatch!");
			}
		}
	});
	registerKeyword("mod_enabled", [this](std::istream& theStream) {
		if (saveGame.parsedMeta)
		{
			commonItems::ignoreItem("unused", theStream);
		}
		else
		{
			// DEFUNCT since 1.31.
			Log(LogLevel::Info) << "-> Detecting used mods.";
			const auto modsList = commonItems::getStrings(theStream);
			Log(LogLevel::Info) << "<> Savegame claims " << modsList.size() << " mods used:";
			Mods mods;
			for (const auto& modPath: modsList)
			{
				Log(LogLevel::Info) << "---> " << modPath;
				mods.emplace_back(Mod("", modPath));
			}
			commonItems::ModLoader modLoader;
			modLoader.loadMods(theConfiguration.getEU4DocumentsPath(), mods);
			theConfiguration.setMods(modLoader.getMods());
		}
	});
	registerKeyword("mods_enabled_names", [this](std::istream& theStream) {
		if (saveGame.parsedMeta)
		{
			commonItems::ignoreItem("unused", theStream);
		}
		else
		{
			// In use since 1.31.
			Log(LogLevel::Info) << "-> Detecting used mods.";
			const auto& modBlobs = commonItems::blobList(theStream);
			Log(LogLevel::Info) << "<> Savegame claims " << modBlobs.getBlobs().size() << " mods used:";
			Mods mods;
			for (const auto& modBlob: modBlobs.getBlobs())
			{
				auto modStream = std::stringstream(modBlob);
				const auto& modName = ModNames(modStream);
				mods.emplace_back(Mod(modName.getName(), modName.getPath()));
				Log(LogLevel::Info) << "---> [" << modName.getName() << "]: " << modName.getPath();
			}
			commonItems::ModLoader modLoader;
			modLoader.loadMods(theConfiguration.getEU4DocumentsPath(), mods);
			theConfiguration.setMods(modLoader.getMods());
			for (const auto& mod: theConfiguration.getMods())
			{
				if (mod.name == "Voltaire's Nightmare")
				{
					Log(LogLevel::Notice) << "Voltaire's Nightmare detected. Enabling VN support.";
					theConfiguration.setVN();
					if (!theConfiguration.isHpmEnabled())
					{
						Log(LogLevel::Notice) << "VN support requires HPM. Enabling HPM automatically.";
						theConfiguration.setHybridMod(Configuration::HYBRIDMOD::HPM);
						if (theConfiguration.isHPMVerified())
						{
							theConfiguration.swapInstallationPathToHPM();
						}
						else
						{
							Log(LogLevel::Error) << "Voltaire's Nightmare uses HPM hybridization.";
							throw std::runtime_error("HPM installation cannot be found in " + theConfiguration.getVic2Path() + "/mod/HPM");
						}
					}
					if (theConfiguration.getEuroCentrism() != Configuration::EUROCENTRISM::EuroCentric)
					{
						Log(LogLevel::Notice) << "VN is auto-enabling Full Colony Annexation and Eurocentric conversion.";
						theConfiguration.setEurocentrism(Configuration::EUROCENTRISM::EuroCentric);
					}
					if (*version < GameVersion("1.31.5"))
						throw std::runtime_error("VN support requires 1.31 saves or higher.");
				}
			}
		}
	});
	// Old Style (up to 1.32)
	registerKeyword("revolution_target", [this](std::istream& theStream) {
		revolutionTargetString = commonItems::getString(theStream);
	});
	// New style (1.33 and onwards)
	registerKeyword("revolution", [this](std::istream& theStream) {
		revolutionParser.parseStream(theStream);
	});
	registerKeyword("celestial_empire", [this](std::istream& theStream) {
		const EU4Empire empireBlock(theStream);
		celestialEmperor = empireBlock.getEmperor();
	});
	registerKeyword("empire", [this](std::istream& theStream) {
		const EU4Empire empireBlock(theStream);
		holyRomanEmperor = empireBlock.getEmperor();
		hreReforms = empireBlock.getHREReforms();
	});
	// Old style of marking the emperor pre-1.20
	registerKeyword("emperor", [this](std::istream& theStream) {
		holyRomanEmperor = commonItems::getString(theStream);
	});
	registerKeyword("provinces", [this](std::istream& theStream) {
		Log(LogLevel::Info) << "-> Loading Provinces";
		provinces = std::make_unique<Provinces>(theStream);

		const auto& possibleDate = provinces->getProvince(1)->getFirstOwnedDate();
		if (possibleDate)
			theConfiguration.setFirstEU4Date(*possibleDate);
	});
	registerKeyword("countries", [this, ideaEffectMapper](std::istream& theStream) {
		Log(LogLevel::Info) << "-> Loading Countries";
		const Countries processedCountries(*version, theStream, ideaEffectMapper);
		theCountries = processedCountries.getTheCountries();
	});
	registerKeyword("diplomacy", [this](std::istream& theStream) {
		Log(LogLevel::Info) << "-> Loading Diplomacy";
		const EU4Diplomacy theDiplomacy(theStream);
		diplomacy = theDiplomacy.getAgreements();
		Log(LogLevel::Info) << "-> Loaded " << diplomacy.size() << " agreements";
	});
	registerKeyword("active_war", [this](std::istream& theStream) {
		const War newWar(theStream);
		wars.push_back(newWar);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void EU4::World::markNewWorldCountries()
{
	for (const auto& country: theCountries | std::views::values)
	{
		const auto capital = country->getCapital();
		if (capital == 0)
			continue;
		const auto& superRegion = regions->getParentSuperRegionName(capital);
		if (!superRegion)
			continue;
		const auto& superGroup = superGroupMapper.getGroupForSuperRegion(*superRegion);
		if (!superGroup)
			continue;
		if (*superGroup == "new_world" ||
			 *superRegion == "oceania_superregion") // a bit of a hack since oceania isn't a new world region (for neoculture reasons)
			country->setNewWorld(true);
	}
}

void EU4::World::calculateIndustry() const
{
	for (const auto& country: theCountries)
		country.second->buildManufactoryCount(theCountries);
}

void EU4::World::buildPopRatios() const
{
	for (const auto& province: provinces->getAllProvinces())
		province.second->buildPopRatio(superGroupMapper, *regions);
}

void EU4::World::generateNeoCultures()
{
	for (const auto& province: provinces->getAllProvinces())
	{
		for (const auto& popRatio: province.second->getPopRatios())
		{
			// Are we operating within native super region for this culture's pop ratio?
			const auto& superRegionName = regions->getParentSuperRegionName(province.first);
			if (!superRegionName)
				continue;
			const auto& currentCulture = popRatio.getCulture();
			if (nativeCultures[*superRegionName].count(currentCulture))
				continue;

			// Are we already a neoculture? Bail if so.
			if (!popRatio.getOriginalCulture().empty())
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
	const auto& cultureGroup = cultureGroupsMapper->getGroupForCulture(oldCultureName);
	if (!cultureGroup)
	{
		// Bail gracefully.
		Log(LogLevel::Warning) << "Culture " << oldCultureName << " has no culture group defined! This should not happen!";
		return oldCultureName;
	}

	// This is the new culture name.
	auto neoCultureName = cultureGroup->getName() + "_" + superRegionName + "_culture";

	// Grab culture definitions.
	const auto& cultureItr = cultureGroup->getCultures().find(oldCultureName);
	if (cultureItr == cultureGroup->getCultures().end())
	{
		// what is going in in there?
		Log(LogLevel::Warning) << "Culture " << oldCultureName << " has no culture definitions! This should not happen!";
		return oldCultureName;
	}
	const auto neoCulture = cultureItr->second;

	// We may already have this neoCulture registered (generated by another culture within the same group).
	const auto& neoCultureItr = cultureGroup->getCultures().find(neoCultureName);
	if (neoCultureItr == cultureGroup->getCultures().end())
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

void EU4::World::verifySave()
{
	const auto& savePath = theConfiguration.getEU4SaveGamePath();
	const std::ifstream saveFile(std::filesystem::u8path(savePath), std::ios::in | std::ios::binary);
	std::stringstream inStream;
	inStream << saveFile.rdbuf();
	saveGame.gamestate = inStream.str();

	const auto save = rakaly::parseEu4(saveGame.gamestate);
	if (const auto& melt = save.meltMeta(); melt)
	{
		Log(LogLevel::Info) << "Meta extracted successfully.";
		melt->writeData(saveGame.metadata);
	}
	else if (save.is_binary())
	{
		Log(LogLevel::Error) << "Binary Save and NO META!";
	}

	if (save.is_binary())
	{
		Log(LogLevel::Info) << "Gamestate is binary, melting.";
		const auto& melt = save.melt();
		if (melt.has_unknown_tokens())
		{
			Log(LogLevel::Error) << "Rakaly reports errors while melting ironman save!";
		}

		melt.writeData(saveGame.gamestate);
	}
	else
	{
		Log(LogLevel::Info) << "Gamestate is textual.";
		const auto& melt = save.melt();
		melt.writeData(saveGame.gamestate);
	}

	zip_t* zip = zip_open(savePath.c_str(), 0, 'r');
	const auto entriesCount = zip_entries_total(zip);
	if (entriesCount > 3)
		throw std::runtime_error("Unrecognized savegame structure! RNW savegames are NOT supported!");

	// Always dump to disk for easier debug.
	std::ofstream metaDump("metaDump.txt");
	metaDump << saveGame.metadata;
	metaDump.close();

	std::ofstream saveDump("saveDump.txt");
	saveDump << saveGame.gamestate;
	saveDump.close();
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

void EU4::World::buildProvinceWeights() const
{
	for (const auto& province: provinces->getAllProvinces())
		province.second->determineProvinceWeight(*buildingTypes);
}

void EU4::World::addProvinceInfoToCountries()
{
	// add province owner info to countries
	for (const auto& province: provinces->getAllProvinces())
		if (const auto& owner = theCountries.find(province.second->getOwnerString()); owner != theCountries.end())
			owner->second->addProvince(province.second);

	// add province core info to countries
	for (const auto& province: provinces->getAllProvinces())
		for (const auto& core: province.second->getCores())
			if (const auto& country = theCountries.find(core); country != theCountries.end())
				country->second->addCore(province.second);
}

void EU4::World::loadRegions()
{
	if (*version >= GameVersion("1.14"))
		loadEU4RegionsNewVersion();
	else
		loadEU4RegionsOldVersion();
}

void EU4::World::assignProvincesToAreas(const std::map<std::string, std::set<int>>& theAreas) const
{
	for (const auto& [areaName, provinceIDs]: theAreas)
	{
		for (auto provNum: provinceIDs)
		{
			try
			{
				provinces->getProvince(provNum)->setArea(areaName);
			}
			catch (std::exception& e)
			{
				Log(LogLevel::Warning) << e.what();
			}
		}
	}
}

void EU4::World::loadEU4RegionsOldVersion()
{
	auto regionFilename = theConfiguration.getEU4Path() + "/map/region.txt";

	for (const auto& mod: theConfiguration.getMods())
	{
		if (!commonItems::DoesFileExist(mod.path + "/map/region.txt"))
			continue;
		regionFilename = mod.path + "/map/region.txt";
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
	for (const auto& mod: theConfiguration.getMods())
	{
		if (!commonItems::DoesFileExist(mod.path + "/map/area.txt"))
			continue;
		areaFilename = mod.path + "/map/area.txt";
	}
	for (const auto& mod: theConfiguration.getMods())
	{
		if (!commonItems::DoesFileExist(mod.path + "/map/region.txt"))
			continue;
		regionFilename = mod.path + "/map/region.txt";
	}
	for (const auto& mod: theConfiguration.getMods())
	{
		if (!commonItems::DoesFileExist(mod.path + "/map/superregion.txt"))
			continue;
		superRegionFilename = mod.path + "/map/superregion.txt";
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
	std::ifstream commonCountries(fs::u8path(theConfiguration.getEU4Path() + "/common/country_tags/00_countries.txt"));
	if (!commonCountries.is_open())
		throw std::runtime_error("Could not open " + theConfiguration.getEU4Path() + "/common/country_tags/00_countries.txt!");
	readCommonCountriesFile(commonCountries, theConfiguration.getEU4Path());

	for (const auto& mod: theConfiguration.getMods())
		for (const auto& fileName: commonItems::GetAllFilesInFolder(mod.path + "/common/country_tags/"))
		{
			std::ifstream convertedCommonCountries(fs::u8path(mod.path + "/common/country_tags/" + fileName));
			if (!convertedCommonCountries.is_open())
				throw std::runtime_error("Could not open " + mod.path + "/common/country_tags/" + fileName + "!");
			readCommonCountriesFile(convertedCommonCountries, mod.path);
		}
}

void EU4::World::readCommonCountriesFile(std::istream& in, const std::string& rootPath)
{
	std::string line;
	while (!in.eof())
	{
		std::getline(in, line);
		auto countryLine = line;
		if (countryLine.size() >= 6 && countryLine[0] != '#' && countryLine[1] != '#')
		{
			// First three characters must be the tag.
			auto tag = countryLine.substr(0, 3);
			if (const auto& countryItr = theCountries.find(tag); countryItr != theCountries.end())
			{
				const auto& country = countryItr->second;

				// Dropping comment after line
				const auto commentPos = countryLine.find('#', 3);
				if (commentPos != std::string::npos)
					countryLine = countryLine.substr(0, commentPos);

				const auto equalPos = countryLine.find('=', 3);
				const auto beginPos = countryLine.find_first_not_of(' ', equalPos + 1);
				const auto endPos = countryLine.find_last_of('\"') + 1;
				auto fileName = countryLine.substr(beginPos, endPos - beginPos);
				fileName = commonItems::remQuotes(fileName); // This will clear quotes if any.

				// Parse the country file.
				auto fullFilename = rootPath + "/common/" + fileName;
				auto localFileName = trimPath(fullFilename);
				if (commonItems::DoesFileExist(fullFilename))
				{
					country->readFromCommonCountry(localFileName, fullFilename);
				}
				else
				{
					// Try in vanilla if nothing in mods.
					fullFilename = theConfiguration.getEU4Path() + "/common/" + fileName;
					if (commonItems::DoesFileExist(fullFilename))
						country->readFromCommonCountry(localFileName, fullFilename);
					else
						Log(LogLevel::Warning) << "Where is country file for " << tag << ": " << fullFilename << "?";
				}
			}
		}
	}
}

void EU4::World::setLocalizations()
{
	for (const auto& theCountry: theCountries)
	{
		const auto& nameLocalizations = EU4localization.getTextInEachLanguage(theCountry.second->getTag());
		if (nameLocalizations)
			for (const auto& [language, name]: *nameLocalizations)
				theCountry.second->setLocalizationName(language, name);

		const auto& adjectiveLocalizations = EU4localization.getTextInEachLanguage(theCountry.second->getTag() + "_ADJ");
		if (adjectiveLocalizations)
			for (const auto& [language, adjective]: *adjectiveLocalizations)
				theCountry.second->setLocalizationAdjective(language, adjective);
	}
}

void EU4::World::resolveRegimentTypes()
{
	for (const auto& country: theCountries)
		country.second->resolveRegimentTypes(unitTypeMapper);
}

void EU4::World::mergeNations()
{
	const NationMergeParser mergeParser;

	if (mergeParser.getMergeDaimyos())
		uniteJapan();

	for (const auto& mergeBlock: mergeParser.getMergeBlocks())
		if (mergeBlock.getMerge() && !mergeBlock.getMaster().empty())
		{
			Log(LogLevel::Info) << "- Merging nations for: " << mergeBlock.getMaster();
			auto master = getCountry(mergeBlock.getMaster());
			for (const auto& slaveTag: mergeBlock.getSlaves())
			{
				const auto& slave = getCountry(slaveTag);
				master->eatCountry(slave);
			}
		}

	// We must also merge all colonial nations.
	if (theConfiguration.isVN())
	{
		mappers::AgreementMapper agreementMapper;
		for (const auto& agreement: diplomacy)
		{
			if (agreementMapper.isAgreementInColonies(agreement.getAgreementType()))
			{
				const auto& overlord = getCountry(agreement.getOriginTag());
				if (!overlord)
					continue;
				const auto& colony = getCountry(agreement.getTargetTag());
				if (!colony)
					continue;
				overlord->eatCountry(colony);
			}
		}
	}
}

void EU4::World::uniteJapan()
{
	Log(LogLevel::Info) << "-> Uniting Japan";

	std::shared_ptr<Country> japan;

	if (*version >= GameVersion("1.20.0.0"))
	{
		for (const auto& country: theCountries)
		{
			if (country.second->getPossibleShogun())
			{
				const auto& tag = country.first;
				Log(LogLevel::Info) << "- " << tag << " is the shogun.";
				japan = getCountry(tag);
				break;
			}
		}
	}
	else
	{
		japan = getCountry("JAP");
	}
	if (!japan)
		return;
	if (japan->hasFlag("united_daimyos_of_japan"))
		return;

	for (const auto& country: theCountries)
		if (country.second->getPossibleDaimyo())
			japan->eatCountry(country.second);
}

void EU4::World::removeEmptyNations()
{
	std::map<std::string, std::shared_ptr<Country>> survivingCountries;

	for (const auto& country: theCountries)
	{
		const auto& countryProvinces = country.second->getProvinces();
		const auto& countryCores = country.second->getCores();
		bool isShatteredHreTag = country.second->getTag() == shatteredHreTag;
		if (!countryProvinces.empty() || !countryCores.empty() || isShatteredHreTag)
			survivingCountries.insert(country);
	}

	theCountries = survivingCountries;
}

void EU4::World::removeDeadLandlessNations()
{
	std::map<std::string, std::shared_ptr<Country>> landlessCountries;
	for (const auto& country: theCountries)
		if (country.second->getProvinces().empty())
			landlessCountries.insert(country);

	for (const auto& country: landlessCountries)
	{
		bool isShatteredHreTag = country.second->getTag() == shatteredHreTag;
		if (!country.second->cultureSurvivesInCores(theCountries) && !isShatteredHreTag)
			theCountries.erase(country.first);
	}
}

void EU4::World::removeLandlessNations()
{
	std::map<std::string, std::shared_ptr<Country>> survivingCountries;

	for (const auto& country: theCountries)
	{
		bool isShatteredHreTag = country.second->getTag() == shatteredHreTag;
		if (const auto& theProvinces = country.second->getProvinces(); !theProvinces.empty() || isShatteredHreTag)
			survivingCountries.insert(country);
	}

	theCountries = survivingCountries;
}

void EU4::World::setEmpires()
{
	for (const auto& country: theCountries)
	{
		// set HRE stuff
		if (country.second->getCapital() != 0 && provinces->getProvince(country.second->getCapital())->inHre())
			country.second->setInHRE(true);
		if (country.second->getTag() == holyRomanEmperor)
			country.second->setEmperor(true);
		if (country.second->getTag() == celestialEmperor)
			country.second->setCelestialEmperor(true);
	}
}

std::shared_ptr<EU4::Country> EU4::World::getCountry(const std::string& tag) const
{
	const auto& country = theCountries.find(tag);
	if (country != theCountries.end())
		return country->second;
	else
		return nullptr;
}

void EU4::World::importShatteredHreTag()
{
	for (const auto& mod: theConfiguration.getMods())
	{
		const auto& modFiles = commonItems::GetAllFilesInFolder(mod.path);
		for (const auto& file: modFiles)
		{
			if (file == "i_am_hre.txt")
			{
				std::ifstream inStream(mod.path + "i_am_hre.txt");
				std::ostringstream incomingTag;
				incomingTag << inStream.rdbuf();
				inStream.close();
				shatteredHreTag = incomingTag.str();
			}
		}
	}
}