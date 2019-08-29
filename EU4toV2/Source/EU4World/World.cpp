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



#include "World.h"
#include "Buildings/Buildings.h"
#include "Countries.h"
#include "CultureGroups.h"
#include "EU4Country.h"
#include "EU4Diplomacy.h"
#include "EU4Version.h"
#include "EU4Localisation.h"
#include "Mods/Mod.h"
#include "Mods/Mods.h"
#include "Provinces/EU4Province.h"
#include "Regions/Areas.h"
#include "../Configuration.h"
#include "../Mappers/Ideas/IdeaEffectMapper.h"
#include "../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "../Mappers/ReligionMapper.h"
#include "Log.h"
#include "NewParserToOldParserConverters.h"
#include "Object.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include "ParadoxParserUTF8.h"
#include <set>
#include <algorithm>
#include <exception>
#include <fstream>
#include <string>



EU4::world::world(const string& EU4SaveFileName, const mappers::IdeaEffectMapper& ideaEffectMapper):
	theCountries()
{
	registerKeyword(std::regex("EU4txt"), [this](const std::string& unused, std::istream& theStream){});
	registerKeyword(std::regex("date"), [this](const std::string& dateText, std::istream& theStream)
		{
			commonItems::singleString dateString(theStream);
			date endDate(dateString.getString());
			theConfiguration.setLastEU4Date(endDate);
		}
	);
	registerKeyword(std::regex("savegame_version"), [this](const std::string& versionText, std::istream& theStream)
		{
			version = std::make_unique<EU4::Version>(theStream);
			theConfiguration.setEU4Version(*version);
		}
	);
	registerKeyword(std::regex("dlc_enabled"), [this](const std::string& DLCText, std::istream& theStream)
		{
			auto versionsObject = commonItems::convert8859Object(DLCText, theStream);
			loadActiveDLC(versionsObject);
		}
	);
	registerKeyword(std::regex("mod_enabled"), [this](const std::string& modText, std::istream& theStream) {
		Mods theMods(theStream, theConfiguration);
	});
	registerKeyword(std::regex("revolution_target"), [this](const std::string& revolutionText, std::istream& theStream)
		{
			auto modsObject = commonItems::convert8859String(revolutionText, theStream);
			loadRevolutionTargetString(modsObject);
		}
	);
	registerKeyword(std::regex("empire"), [this](const std::string& empireText, std::istream& theStream)
		{
			auto empireObject = commonItems::convert8859Object(empireText, theStream);
			loadEmpires(empireObject);
		}
	);
	registerKeyword(std::regex("emperor"), [this](const std::string& emperorText, std::istream& theStream)
		{
			auto emperorObject = commonItems::convert8859Object(emperorText, theStream);
			loadEmpires(emperorObject);
		}
	);
	registerKeyword(std::regex("celestial_empire"), [this](const std::string& empireText, std::istream& theStream)
		{
			auto empireObject = commonItems::convert8859Object(empireText, theStream);
			loadEmpires(empireObject);
		}
	);
	registerKeyword(std::regex("provinces"), [this](const std::string& provincesText, std::istream& theStream) {
		std::ifstream buildingsFile(theConfiguration.getEU4Path() + "/common/buildings/00_buildings.txt");
		Buildings buildingTypes(buildingsFile);
		buildingsFile.close();

		provinces = std::make_unique<Provinces>(theStream, buildingTypes);
		std::optional<date> possibleDate = provinces->getProvince(1).getFirstOwnedDate();
		if (possibleDate)
		{
			theConfiguration.setFirstEU4Date(*possibleDate);
		}
	});
	registerKeyword(
		std::regex("countries"),
		[this, ideaEffectMapper](const std::string& countriesText, std::istream& theStream)
		{
			loadCountries(theStream, ideaEffectMapper);
		}
	);
	registerKeyword(std::regex("diplomacy"), [this](const std::string& diplomacyText, std::istream& theStream) {
		auto diplomacyObject = commonItems::convert8859Object(diplomacyText, theStream);
		loadDiplomacy(diplomacyObject);
	});
	registerKeyword(std::regex("[A-Za-z0-9\\_]+"), commonItems::ignoreItem);

	if (!diplomacy)
	{
		auto nullDiploObject = std::make_shared<Object>("");
		diplomacy = std::make_unique<EU4Diplomacy>(nullDiploObject);
	}

	LOG(LogLevel::Info) << "* Importing EU4 save *";
	verifySave(EU4SaveFileName);
	parseFile(EU4SaveFileName);

	LOG(LogLevel::Info) << "Building world";
	setEmpires();
	addProvinceInfoToCountries();
	provinces->determineTotalProvinceWeights(theConfiguration);
	loadRegions();
	readCommonCountries();
	setLocalisations();
	resolveRegimentTypes();
	mergeNations();

	loadRevolutionTarget();

	importReligions();

	removeEmptyNations();
	if (theConfiguration.getRemovetype() == "dead")
	{
		removeDeadLandlessNations();
	}
	else if (theConfiguration.getRemovetype() == "all")
	{
		removeLandlessNations();
	}
}


void EU4::world::verifySave(const std::string& EU4SaveFileName)
{
	std::ifstream saveFile(EU4SaveFileName);
	if (!saveFile.is_open())
	{
		std::runtime_error exception("Could not open save! Exiting!");
		throw exception;
	}
	else
	{
		char buffer[7];
		saveFile.get(buffer, 7);
		if ((buffer[0] == 'P') && (buffer[1] == 'K'))
		{
			std::runtime_error exception("Saves must be uncompressed to be converted.");
			throw exception;
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
			std::runtime_error exception("Ironman saves cannot be converted.");
			throw exception;
		}
	}

	saveFile.close();
}


void EU4::world::loadActiveDLC(const shared_ptr<Object> EU4SaveObj)
{
	vector<shared_ptr<Object>> enabledDLCsObj = EU4SaveObj->getValue("dlc_enabled");
	if (enabledDLCsObj.size() > 0)
	{
		vector<string>	activeDLCs;
		vector<string> DLCsObj = enabledDLCsObj[0]->getTokens();
		for (auto DLCsItr: DLCsObj)
		{
			activeDLCs.push_back(DLCsItr);
		}

		theConfiguration.setActiveDLCs(activeDLCs);
	}
}


void EU4::world::loadEmpires(const shared_ptr<Object> EU4SaveObj)
{
	vector<shared_ptr<Object>> emperorObj = EU4SaveObj->getValue("emperor");
	if (emperorObj.size() > 0)
	{
		holyRomanEmperor = emperorObj[0]->getLeaf();
	}

	vector<shared_ptr<Object>> empireObj = EU4SaveObj->getValue("empire");
	if (empireObj.size() > 0)
	{
		loadHolyRomanEmperor(empireObj);
	}
	vector<shared_ptr<Object>> celestialEmpireObj = EU4SaveObj->getValue("celestial_empire");
	if (celestialEmpireObj.size() > 0)
	{
		loadCelestialEmperor(celestialEmpireObj);
	}
}

void EU4::world::loadHolyRomanEmperor(vector<shared_ptr<Object>> empireObj)
{
	vector<shared_ptr<Object>> emperorObj = empireObj[0]->getValue("emperor");
	if (emperorObj.size() > 0)
	{
		holyRomanEmperor = emperorObj[0]->getLeaf();
	}
}


void EU4::world::loadCelestialEmperor(vector<shared_ptr<Object>> celestialEmpireObj)
{
	vector<shared_ptr<Object>> emperorObj = celestialEmpireObj[0]->getValue("emperor");
	if (emperorObj.size() > 0)
	{
		celestialEmperor = emperorObj[0]->getLeaf();
	}
}


void EU4::world::loadCountries(istream& theStream, const mappers::IdeaEffectMapper& ideaEffectMapper)
{
	countries processedCountries(*version, theStream, ideaEffectMapper);
	auto theProcessedCountries = processedCountries.getTheCountries();
	theCountries.swap(theProcessedCountries);
}


void EU4::world::loadRevolutionTargetString(const shared_ptr<Object> EU4SaveObj)
{
	vector<shared_ptr<Object>> revolutionTargetObj = EU4SaveObj->getValue("revolution_target");
	if (revolutionTargetObj.size() > 0)
	{
		revolutionTargetString = revolutionTargetObj[0]->getLeaf();
	}
}


void EU4::world::loadRevolutionTarget()
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


void EU4::world::addProvinceInfoToCountries()
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


void EU4::world::loadDiplomacy(const shared_ptr<Object> EU4SaveObj)
{
	vector<shared_ptr<Object>> diploObj = EU4SaveObj->getValue("diplomacy");	// the object holding the world's diplomacy
	if (diploObj.size() > 0)
	{
		diplomacy = std::make_unique<EU4Diplomacy>(diploObj[0]);
	}
	else
	{
		diplomacy = std::make_unique<EU4Diplomacy>();
	}
}


void EU4::world::loadRegions()
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


void EU4::world::loadEU4RegionsOldVersion()
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
	EU4::areas installedAreas(theStream);
	theStream.close();

	regions = std::make_unique<Regions>(installedAreas);
}


void EU4::world::loadEU4RegionsNewVersion()
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
	EU4::areas installedAreas(areaStream);
	areaStream.close();

	std::ifstream regionStream(regionFilename);
	regions = std::make_unique<Regions>(installedAreas, regionStream);
	regionStream.close();
}


void EU4::world::checkAllEU4CulturesMapped(const mappers::CultureMapper& cultureMapper) const
{
	for (auto cultureItr: EU4::cultureGroups::getCultureToGroupMap())
	{
		string Vi2Culture;
		string EU4Culture = cultureItr.first;

		std::optional<std::string> matched = cultureMapper.cultureMatch(*regions, EU4Culture, "");
		if (!matched)
		{
			LOG(LogLevel::Warning) << "No culture mapping for EU4 culture " << EU4Culture;
		}
	}
}


void EU4::world::readCommonCountries()
{
	LOG(LogLevel::Info) << "Reading EU4 common/countries";
	ifstream commonCountries(theConfiguration.getEU4Path() + "/common/country_tags/00_countries.txt");	// the data in the countries file
	readCommonCountriesFile(commonCountries, theConfiguration.getEU4Path());
	for (auto itr: theConfiguration.getEU4Mods())
	{
		set<string> fileNames;
		Utils::GetAllFilesInFolder(itr + "/common/country_tags/", fileNames);
		for (set<string>::iterator fileItr = fileNames.begin(); fileItr != fileNames.end(); fileItr++)
		{
			ifstream convertedCommonCountries(itr + "/common/country_tags/" + *fileItr);	// a stream of the data in the converted countries file
			readCommonCountriesFile(convertedCommonCountries, itr);
		}
	}
}


void EU4::world::readCommonCountriesFile(istream& in, const std::string& rootPath)
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
				if (commentPos != string::npos)
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
				std::string localFileName = fullFilename.substr(lastPathSeparatorPos + 1, string::npos);
				if (Utils::DoesFileExist(fullFilename))
				{
					country->readFromCommonCountry(localFileName, fullFilename);
				}
			}
		}
	}
}


void EU4::world::setLocalisations()
{
	LOG(LogLevel::Info) << "Reading localisation";
	EU4Localisation localisation;
	localisation.ReadFromAllFilesInFolder(theConfiguration.getEU4Path() + "/localisation");
	for (auto itr: theConfiguration.getEU4Mods())
	{
		LOG(LogLevel::Debug) << "Reading mod localisation";
		localisation.ReadFromAllFilesInFolder(itr + "/localisation");
	}

	for (auto theCountry: theCountries)
	{
		const auto& nameLocalisations = localisation.GetTextInEachLanguage(theCountry.second->getTag());	// the names in all languages
		for (const auto& nameLocalisation : nameLocalisations)	// the name under consideration
		{
			const std::string& language = nameLocalisation.first;	// the language
			const std::string& name = nameLocalisation.second;		// the name of the country in this language
			theCountry.second->setLocalisationName(language, name);
		}
		const auto& adjectiveLocalisations = localisation.GetTextInEachLanguage(theCountry.second->getTag() + "_ADJ");	// the adjectives in all languages
		for (const auto& adjectiveLocalisation : adjectiveLocalisations)	// the adjective under consideration
		{
			const std::string& language = adjectiveLocalisation.first;		// the language
			const std::string& adjective = adjectiveLocalisation.second;	// the adjective for the country in this language
			theCountry.second->setLocalisationAdjective(language, adjective);
		}
	}
}


void EU4::world::resolveRegimentTypes()
{
	LOG(LogLevel::Info) << "Resolving unit types.";
	RegimentTypeMap rtm;
	fstream read;
	read.open("unit_strength.txt");
	if (read.is_open())
	{
		read.close();
		read.clear();
		LOG(LogLevel::Info) << "\tReading unit strengths from unit_strength.txt";
		shared_ptr<Object> unitsObj = parser_UTF8::doParseFile("unit_strength.txt");
		if (unitsObj == NULL)
		{
			LOG(LogLevel::Error) << "Could not parse file unit_strength.txt";
			exit(-1);
		}
		for (int i = 0; i < num_reg_categories; ++i)
		{
			AddCategoryToRegimentTypeMap(unitsObj, (RegimentCategory)i, RegimentCategoryNames[i], rtm);
		}
	}
	else
	{
		LOG(LogLevel::Info) << "\tReading unit strengths from EU4 installation folder";

		set<string> filenames;
		Utils::GetAllFilesInFolder(theConfiguration.getEU4Path() + "/common/units/", filenames);
		for (auto filename: filenames)
		{
			AddUnitFileToRegimentTypeMap((theConfiguration.getEU4Path() + "/common/units"), filename, rtm);
		}
	}
	read.close();
	read.clear();

	for (auto itr = theCountries.begin(); itr != theCountries.end(); ++itr)
	{
		itr->second->resolveRegimentTypes(rtm);
	}
}


// todo: move the getting of rules into its own mapper, with a merge rule structure type
//		then break out things into subfunctions with a give rule as a parameter
void EU4::world::mergeNations()
{
	LOG(LogLevel::Info) << "Merging nations";
	shared_ptr<Object> mergeObj = parser_UTF8::doParseFile("merge_nations.txt");
	if (mergeObj == NULL)
	{
		LOG(LogLevel::Error) << "Could not parse file merge_nations.txt";
		exit(-1);
	}

	vector<shared_ptr<Object>> rules = mergeObj->getValue("merge_nations");
	if (rules.size() < 0)
	{
		LOG(LogLevel::Debug) << "No nations have merging requested (skipping)";
		return;
	}

	rules = rules[0]->getLeaves();
	for (auto rule: rules)
	{
		if ((rule->getKey() == "merge_daimyos") && (rule->getLeaf() == "yes"))
		{
			uniteJapan();
			continue;
		}

		vector<shared_ptr<Object>> ruleItems = rule->getLeaves();

		string masterTag;
		vector<string> slaveTags;
		bool enabled = false;
		for (auto item: ruleItems)
		{
			if ((item->getKey() == "merge") && (item->getLeaf() == "yes"))
			{
				enabled = true;
			}
			else if (item->getKey() == "master")
			{
				masterTag = item->getLeaf();
			}
			else if (item->getKey() == "slave")
			{
				slaveTags.push_back(item->getLeaf());
			}
		}

		auto master = getCountry(masterTag);
		if (enabled && master)
		{
			for (auto slaveTag: slaveTags)
			{
				auto slave = getCountry(slaveTag);
				if (slave)
				{
					master->eatCountry(slave, master);
				}
			}
		}
	}
}


void EU4::world::uniteJapan()
{
	shared_ptr<EU4::Country> japan;

	auto version20 = EU4::Version("1.20.0.0");
	if (*version >= version20)
	{
		for (auto country : theCountries)
		{
			if (country.second->getPossibleShogun())
			{
				string tag = country.first;
				japan = getCountry(tag);
			}
		}
	}
	else 
	{
		japan = getCountry("JAP");
	}
	if (japan == nullptr)
	{
		return;
	}
	if (japan->hasFlag("united_daimyos_of_japan"))
	{
		return;
	}

	for (auto country: theCountries)
	{
		if (country.second->getPossibleDaimyo())
		{
			japan->eatCountry(country.second, japan);
		}
	}
}


void EU4::world::checkAllEU4ReligionsMapped(const mappers::ReligionMapper& religionMapper) const
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


void EU4::world::checkAllProvincesMapped(const mappers::ProvinceMapper& provinceMapper) const
{
	provinces->checkAllProvincesMapped(provinceMapper);
}


void EU4::world::importReligions()
{
	LOG(LogLevel::Info) << "Parsing EU4 religions";

	std::ifstream religionsFile(theConfiguration.getEU4Path() + "/common/religions/00_religion.txt");
	theReligions.addReligions(religionsFile);
	religionsFile.close();

	for (auto modName: theConfiguration.getEU4Mods())
	{
		std::set<std::string> filenames;
		Utils::GetAllFilesInFolder(modName + "/common/religions/", filenames);
		for (auto filename: filenames)
		{
			std::ifstream modReligionsFile(modName + "/common/religions/" + filename);
			theReligions.addReligions(modReligionsFile);
			modReligionsFile.close();
		}
	}
}


void EU4::world::removeEmptyNations()
{
	std::map<std::string, std::shared_ptr<EU4::Country>> survivingCountries;

	for (auto country: theCountries)
	{
		auto countryProvinces = country.second->getProvinces();
		auto countryCores = country.second->getCores();
		if ((countryProvinces.size() == 0) && (countryCores.size() == 0))
		{
			LOG(LogLevel::Debug) << "Removing empty nation " << country.first;
		}
		else
		{
			survivingCountries.insert(country);
		}
	}

	theCountries.swap(survivingCountries);
}


void EU4::world::removeDeadLandlessNations()
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
			LOG(LogLevel::Debug) << "Removing dead landless nation " << country.first;
		}
	}
}


void EU4::world::removeLandlessNations()
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


void EU4::world::setEmpires()
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


std::shared_ptr<EU4::Country> EU4::world::getCountry(std::string tag) const
{
	auto i = theCountries.find(tag);
	if (i != theCountries.end())
	{
		return i->second;
	}
	else
	{
		return {};
	}
}


const EU4::Province& EU4::world::getProvince(int provNum) const
{
	return provinces->getProvince(provNum);
}


bool EU4::world::isRandomWorld() const
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