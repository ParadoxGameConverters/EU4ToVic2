#include "World.h"
#include "Country/Countries.h"
#include "Country/EU4Country.h"
#include "EU4Version.h"
#include "Localisation/EU4Localisation.h"
#include "Mods/Mods.h"
#include "Provinces/EU4Province.h"
#include "Regions/Areas.h"
#include "../Configuration.h"
#include "../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include "NationMerger/NationMergeParser.h"
#include <set>
#include <algorithm>
#include <exception>
#include <fstream>
#include <string>
#include "Relations/EU4Empire.h"

EU4::World::World(const std::string& EU4SaveFileName, const mappers::IdeaEffectMapper& ideaEffectMapper)
{
	registerKeyword("EU4txt", [](const std::string& unused, std::istream& theStream) {});
	registerKeyword("date", [](const std::string& dateText, std::istream& theStream)
		{
			commonItems::singleString dateString(theStream);
			date endDate(dateString.getString());
			theConfiguration.setLastEU4Date(endDate);
		});
	registerKeyword("start_date", [](const std::string& dateText, std::istream& theStream)
		{
			commonItems::singleString startDateString(theStream);
			date startDate(startDateString.getString());
			theConfiguration.setStartEU4Date(startDate);
		});
	registerRegex("(multiplayer_)?random_seed", [](const std::string& key, std::istream& theStream)
		{
			commonItems::singleString randomSeed(theStream);
			theConfiguration.setEU4RandomSeed(stoi(randomSeed.getString().substr(randomSeed.getString().size() - 5)));
		});
	registerKeyword("savegame_version", [this](const std::string& versionText, std::istream& theStream)
		{
			version = std::make_unique<EU4::Version>(theStream);
			theConfiguration.setEU4Version(*version);
		});
	registerKeyword("dlc_enabled", [](const std::string& DLCText, std::istream& theStream)
		{
			commonItems::stringList theDLCs(theStream);
			theConfiguration.setActiveDLCs(theDLCs.getStrings());
		});
	registerKeyword("mod_enabled", [](const std::string& modText, std::istream& theStream) 
		{
			commonItems::stringList modList(theStream);
			Mods theMods(modList.getStrings(), theConfiguration);
		});
	registerKeyword("revolution_target", [this](const std::string& revolutionText, std::istream& theStream)
		{
			commonItems::singleString revTargetStr(theStream);
			revolutionTargetString = revTargetStr.getString();
		});
	registerKeyword("celestial_empire", [this](const std::string& empireText, std::istream& theStream)
		{
			EU4::EU4Empire empireBlock(theStream);
			celestialEmperor = empireBlock.getEmperor();
		});
	registerKeyword("empire", [this](const std::string& empireText, std::istream& theStream)
		{
			EU4::EU4Empire empireBlock(theStream);
			holyRomanEmperor = empireBlock.getEmperor();
		});
	registerKeyword("emperor", [this](const std::string& emperorText, std::istream& theStream)
		{
			commonItems::singleString emperorStr(theStream);
			holyRomanEmperor = emperorStr.getString();
		});
	registerKeyword("provinces", [this](const std::string& provincesText, std::istream& theStream) 
		{
			LOG(LogLevel::Info) << "- Loading Provinces";
			modifierTypes.initialize();
			provinces = std::make_unique<Provinces>(theStream, buildingTypes, modifierTypes);
			std::optional<date> possibleDate = provinces->getProvince(1).getFirstOwnedDate();
			if (possibleDate)
			{
				theConfiguration.setFirstEU4Date(*possibleDate);
			}
		});
	registerKeyword("countries", [this, ideaEffectMapper](const std::string& countriesText, std::istream& theStream)
		{
			LOG(LogLevel::Info) << "- Loading Countries";
			countries processedCountries(*version, theStream, ideaEffectMapper, cultureGroupsMapper);
			auto theProcessedCountries = processedCountries.getTheCountries();
			theCountries.swap(theProcessedCountries);
		});
	registerKeyword("diplomacy", [this](const std::string& unused, std::istream& theStream) 
		{
			LOG(LogLevel::Info) << "- Loading Diplomacy";
			EU4::EU4Diplomacy theDiplomacy(theStream);
			diplomacy = theDiplomacy.getAgreements();
			LOG(LogLevel::Info) << "- Loaded " << diplomacy.size() << " agreements";
		});
	registerKeyword("map_area_data", [](const std::string& unused, std::istream& theStream) 
		{
			LOG(LogLevel::Info) << "- Loading Map Area Data";
			commonItems::ignoreItem(unused, theStream);
			LOG(LogLevel::Info) << " - Ignoring Map Area Data";
		});
	registerRegex("[A-Za-z0-9\\_]+", commonItems::ignoreItem);

	LOG(LogLevel::Info) << "Verifying EU4 save.";
	verifySave(EU4SaveFileName);

	LOG(LogLevel::Info) << "* Importing EU4 save *";
	parseFile(EU4SaveFileName);
	clearRegisteredKeywords();

	LOG(LogLevel::Info) << " * Building world *";
	LOG(LogLevel::Info) << "- Loading Empires";
	setEmpires();

	LOG(LogLevel::Info) << "- Processing Province Info";
	addProvinceInfoToCountries();

	LOG(LogLevel::Info) << "- Eliminating Minorities";
	dropMinoritiesFromCountries();

	LOG(LogLevel::Info) << "- Determining Province Weights";
	provinces->determineTotalProvinceWeights(theConfiguration);

	LOG(LogLevel::Info) << "- Loading Regions";
	loadRegions();

	LOG(LogLevel::Info) << "- Reading Countries";
	readCommonCountries();

	LOG(LogLevel::Info) << " - Setting Localizations";
	setLocalisations();

	LOG(LogLevel::Info) << "- Resolving Regiments";
	resolveRegimentTypes();

	LOG(LogLevel::Info) << " - Merging Nations";
	mergeNations();

	LOG(LogLevel::Info) << " - Viva la revolution!";
	loadRevolutionTarget();

	LOG(LogLevel::Info) << " - Dropping Empty Nations";
	removeEmptyNations();
	if (theConfiguration.getRemoveType() == Configuration::DEADCORES::DeadCores)
	{
		removeDeadLandlessNations();
	}
	else if (theConfiguration.getRemoveType() == Configuration::DEADCORES::AllCores)
	{
		removeLandlessNations();
	}
}

void EU4::World::verifySave(const std::string& EU4SaveFileName)
{
	std::ifstream saveFile(EU4SaveFileName);
	if (!saveFile.is_open())
	{
		throw std::runtime_error("Could not open save! Exiting!");
	}
	else
	{
		char buffer[8];
		saveFile.get(buffer, 8);
		if ((buffer[0] == 'P') && (buffer[1] == 'K'))
		{
			std::runtime_error exception("Saves must be uncompressed to be converted.");
			throw std::runtime_error("Saves must be uncompressed to be converted.");;
		}
		else if (
			(buffer[0] == 'E') &&
			(buffer[1] == 'U') &&
			(buffer[2] == '4') &&
			(buffer[3] == 'b') &&
			(buffer[4] == 'i') &&
			(buffer[5] == 'n') &&
			(buffer[6] == 'M')
		) {
			throw std::runtime_error("Ironman saves cannot be converted.");;
		}
	}

	saveFile.close();
}

void EU4::World::loadRevolutionTarget()
{
	if (revolutionTargetString != "")
	{
		auto country = theCountries.find(revolutionTargetString);
		if (country != theCountries.end())
		{
			country->second->viveLaRevolution(true);
		}
	}
}

void EU4::World::dropMinoritiesFromCountries()
{
	for (const auto& country : theCountries)
	{
		country.second->dropMinorityCultures();
	}
}

void EU4::World::addProvinceInfoToCountries()
{
	// add province owner info to countries
	for (auto& province: provinces->getAllProvinces())
	{
		auto owner = theCountries.find(province.second.getOwnerString());
		if (owner != theCountries.end())
		{
			owner->second->addProvince(&province.second);
		}
	}

	// add province core info to countries
	for (auto& province: provinces->getAllProvinces())
	{
		auto cores = province.second.getCores();
		for (auto core: cores)
		{
			auto country = theCountries.find(core);
			if (country != theCountries.end())
			{
				country->second->addCore(&province.second);
			}
		}
	}
}

void EU4::World::loadRegions()
{
	LOG(LogLevel::Info) << "Parsing EU4 regions";

	if (*version >= EU4::Version("1.14"))
	{
		loadEU4RegionsNewVersion();
	}
	else
	{
		loadEU4RegionsOldVersion();
	}
}

void EU4::World::assignProvincesToAreas(const std::map<std::string, std::set<int>>& theAreas)
{
	for (const auto& area : theAreas)
	{
		std::string areaName = area.first;
		for (const int provNum : area.second)
		{
			try
			{
				auto& province = provinces->getProvince(provNum);
				province.setArea(areaName);
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
	std::string regionFilename = theConfiguration.getEU4Path() + "/map/region.txt";

	for (auto itr: theConfiguration.getEU4Mods())
	{
		if (!Utils::DoesFileExist(itr + "/map/region.txt"))
		{
			continue;
		}

		regionFilename = itr + "/map/region.txt";
	}

	std::ifstream theStream(regionFilename);
	EU4::Areas installedAreas(theStream);
	theStream.close();
	assignProvincesToAreas(installedAreas.getAreas());

	regions = std::make_unique<Regions>(installedAreas);
}


void EU4::World::loadEU4RegionsNewVersion()
{
	std::string areaFilename = theConfiguration.getEU4Path() + "/map/area.txt";
	std::string regionFilename = theConfiguration.getEU4Path() + "/map/region.txt";
	for (auto itr: theConfiguration.getEU4Mods())
	{
		if (!Utils::DoesFileExist(itr + "/map/area.txt") || !Utils::DoesFileExist(itr + "/map/region.txt"))
		{
			continue;
		}

		areaFilename = itr + "/map/area.txt";
		regionFilename = itr + "/map/region.txt";
	}

	std::ifstream areaStream(areaFilename);
	EU4::Areas installedAreas(areaStream);
	areaStream.close();
	assignProvincesToAreas(installedAreas.getAreas());

	std::ifstream regionStream(regionFilename);
	regions = std::make_unique<Regions>(installedAreas, regionStream);
	regionStream.close();
}


void EU4::World::checkAllEU4CulturesMapped(const mappers::CultureMapper& cultureMapper) const
{
	for (auto cultureItr: cultureGroupsMapper.getCultureToGroupMap())
	{
		std::string Vi2Culture;
		std::string EU4Culture = cultureItr.first;

		std::optional<std::string> matched = cultureMapper.cultureMatch(*regions, EU4Culture, "", -1, "");
		if (!matched)
		{
			LOG(LogLevel::Warning) << "No culture mapping for EU4 culture " << EU4Culture;
		}
	}
}


void EU4::World::readCommonCountries()
{
	LOG(LogLevel::Info) << "Reading EU4 common/countries";
	std::ifstream commonCountries(theConfiguration.getEU4Path() + "/common/country_tags/00_countries.txt");	// the data in the countries file
	readCommonCountriesFile(commonCountries, theConfiguration.getEU4Path());
	for (auto itr: theConfiguration.getEU4Mods())
	{
		std::set<std::string> fileNames;
		Utils::GetAllFilesInFolder(itr + "/common/country_tags/", fileNames);
		for (std::set<std::string>::iterator fileItr = fileNames.begin(); fileItr != fileNames.end(); ++fileItr)
		{
			std::ifstream convertedCommonCountries(itr + "/common/country_tags/" + *fileItr);	// a stream of the data in the converted countries file
			readCommonCountriesFile(convertedCommonCountries, itr);
		}
	}
}


void EU4::World::readCommonCountriesFile(std::istream& in, const std::string& rootPath)
{
	// Add any info from common\countries
	const int maxLineLength = 10000;	// the maximum line length
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
			std::string tag = countryLine.substr(0, 3);
			if (auto findIter = theCountries.find(tag); findIter != theCountries.end())
			{
				auto country = findIter->second;

				// The country file name is all the text after the equals sign (possibly in quotes).
				size_t commentPos	= countryLine.find('#', 3);
				if (commentPos != std::string::npos)
				{
					countryLine = countryLine.substr(0, commentPos);
				}
				size_t equalPos	= countryLine.find('=', 3);
				size_t beginPos	= countryLine.find_first_not_of(' ', equalPos + 1);
				size_t endPos		= countryLine.find_last_of('\"') + 1;
				std::string fileName = countryLine.substr(beginPos, endPos - beginPos);
				if (fileName.front() == '"' && fileName.back() == '"')
				{
					fileName = fileName.substr(1, fileName.size() - 2);
				}
				std::replace(fileName.begin(), fileName.end(), '/', '/');

				// Parse the country file.
				std::string fullFilename = rootPath + "/common/" + fileName;
				size_t lastPathSeparatorPos = fullFilename.find_last_of('/');
				std::string localFileName = fullFilename.substr(lastPathSeparatorPos + 1, std::string::npos);
				if (Utils::DoesFileExist(fullFilename))
				{
					country->readFromCommonCountry(localFileName, fullFilename);
				}
			}
		}
	}
}

void EU4::World::setLocalisations()
{
	LOG(LogLevel::Info) << "Reading localisation";
	EU4Localisation localisation;

	for (auto theCountry: theCountries)
	{
		const auto& nameLocalisations = localisation.getTextInEachLanguage(theCountry.second->getTag());	// the names in all languages
		for (const auto& nameLocalisation : nameLocalisations)	// the name under consideration
		{
			const std::string& language = nameLocalisation.first;	// the language
			const std::string& name = nameLocalisation.second;		// the name of the country in this language
			theCountry.second->setLocalisationName(language, name);
		}
		const auto& adjectiveLocalisations = localisation.getTextInEachLanguage(theCountry.second->getTag() + "_ADJ");	// the adjectives in all languages
		for (const auto& adjectiveLocalisation : adjectiveLocalisations)	// the adjective under consideration
		{
			const std::string& language = adjectiveLocalisation.first;		// the language
			const std::string& adjective = adjectiveLocalisation.second;	// the adjective for the country in this language
			theCountry.second->setLocalisationAdjective(language, adjective);
		}
	}
}

void EU4::World::resolveRegimentTypes()
{
	LOG(LogLevel::Info) << "Resolving unit types.";
	for (auto itr = theCountries.begin(); itr != theCountries.end(); ++itr)
	{
		itr->second->resolveRegimentTypes(unitTypeMapper);
	}
}

void EU4::World::mergeNations()
{
	LOG(LogLevel::Info) << "Parsing merge nation rules.";
	NationMergeParser mergeParser;
	
	if (mergeParser.getMergeDaimyos()) uniteJapan();

	for (const auto& mergeBlock : mergeParser.getMergeBlocks())
	{
		if (mergeBlock.getMerge() && !mergeBlock.getMaster().empty())
		{
			LOG(LogLevel::Info) << "- Merging nations for: " << mergeBlock.getMaster();
			auto master = getCountry(mergeBlock.getMaster());
			for (auto slaveTag : mergeBlock.getSlaves())
			{
				auto slave = getCountry(slaveTag);
				master->eatCountry(slave);
			}
		}
	}
}

void EU4::World::uniteJapan()
{
	LOG(LogLevel::Info) << "Uniting Japan";

	std::shared_ptr<EU4::Country> japan;

	auto version20 = EU4::Version("1.20.0.0");
	if (*version >= version20)
	{
		for (auto country : theCountries)
		{
			if (country.second->getPossibleShogun())
			{
				std::string tag = country.first;
				LOG(LogLevel::Info) << "- " << tag << " is the shogun.";
				japan = getCountry(tag);
			}
		}
	}
	else 
	{
		japan = getCountry("JAP");
	}
	if (japan == nullptr) return;
	if (japan->hasFlag("united_daimyos_of_japan")) return;

	for (auto country: theCountries)
	{
		if (country.second->getPossibleDaimyo())
		{
			japan->eatCountry(country.second);			
		}
	}
}

void EU4::World::checkAllEU4ReligionsMapped(const mappers::ReligionMapper& religionMapper) const
{
	for (auto EU4Religion: theReligions.getAllReligions())
	{
		auto Vic2Religion = religionMapper.getVic2Religion(EU4Religion.first);
		if (Vic2Religion == "")
		{
			Log(LogLevel::Warning) << "No religion mapping for EU4 religion " << EU4Religion.first;
		}
	}
}

void EU4::World::checkAllProvincesMapped(const mappers::ProvinceMapper& provinceMapper) const
{
	provinces->checkAllProvincesMapped(provinceMapper);
}

void EU4::World::removeEmptyNations()
{
	std::map<std::string, std::shared_ptr<EU4::Country>> survivingCountries;

	for (auto country: theCountries)
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
	std::map<std::string, std::shared_ptr<EU4::Country>> landlessCountries;
	for (auto country: theCountries)
	{
		auto countryProvinces = country.second->getProvinces();
		if (countryProvinces.size() == 0)
		{
			landlessCountries.insert(country);
		}
	}

	for (auto country: landlessCountries)
	{
		if (!country.second->cultureSurvivesInCores(theCountries))
		{
			theCountries.erase(country.first);
		}
	}
}

void EU4::World::removeLandlessNations()
{
	std::map<std::string, std::shared_ptr<EU4::Country>> survivingCountries;

	for (auto country: theCountries)
	{
		auto provinces = country.second->getProvinces();
		if (provinces.size() == 0)
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
	for (auto country: theCountries)
	{
		// set HRE stuff
		if ((country.second->getCapital() != 0) && (provinces->getProvince(country.second->getCapital()).inHre()))
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

std::shared_ptr<EU4::Country> EU4::World::getCountry(std::string tag) const
{
	auto i = theCountries.find(tag);
	if (i != theCountries.end()) return i->second;
	return nullptr;
}

const EU4::Province& EU4::World::getProvince(int provNum) const
{
	return provinces->getProvince(provNum);
}

bool EU4::World::isRandomWorld() const
{
	bool isRandomWorld = true;

	for (auto sourceCountry: theCountries)
	{
		if (sourceCountry.first[0] != 'D' && sourceCountry.second->getRandomName().empty())
		{
			isRandomWorld = false;
		}
	}

	return isRandomWorld;
}
