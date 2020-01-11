#include "V2Flags.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <random>
#include <fstream>
#include "../EU4World/Country/EU4Country.h"
#include "V2Country.h"
#include "../Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "../Mappers/CK2TitleMapper.h"
#include "../Mappers/ColonyFlagsetMapper.h"
#include "../Mappers/CountryMapping.h"
#include "../Mappers/FlagColorMapper.h"
#include "../FlagUtils.h"


const std::vector<std::string> V2Flags::flagFileSuffixes = { ".tga", "_communist.tga", "_fascist.tga", "_monarchy.tga", "_republic.tga" };

void V2Flags::SetV2Tags(const std::map<std::string, V2Country*>& V2Countries)
{
	LOG(LogLevel::Debug) << "Initializing flags";
	tagMap.clear();

	static std::mt19937 generator(static_cast<int>(std::chrono::system_clock::now().time_since_epoch().count()));

	determineUseableFlags();
	getRequiredTags(V2Countries);
	mapTrivialTags();

	std::vector<V2Country*> colonialFail;
	
	// Get the CK2 and colonial flags.
	for (std::map<std::string, V2Country*>::const_iterator i = V2Countries.begin(); i != V2Countries.end(); ++i)
	{
		V2Country* v2source = i->second;
		std::string religion = v2source->getReligion();

		if (i->second->getSourceCountry()
			&& requiredTags.find(i->first) != requiredTags.end())
		{
			auto ck2title = mappers::CountryMappings::getCK2Title(i->first,i->second->getLocalName(),usableFlagTags);
			if ((ck2title) && (usableFlagTags.find(*ck2title) != usableFlagTags.end()))
			{
				tagMap[i->first] = *ck2title;
				usableFlagTags.erase(*ck2title);
				requiredTags.erase(i->first);
			}
			else // try something patronymic
			{
				if (!isalpha(i->first[0]) || !isdigit(i->first[1]) || !isdigit(i->first[2]))
					continue;

				std::string religion = i->second->getReligion();
				std::optional<std::string> randomCK2title;

				// Yay hardcoded paths. If I get round to it, I'll point these at religion.txt instead.
				if (religion == "sunni" || religion == "shiite" || religion == "ibadi")
				{
					randomCK2title = mappers::CK2TitleMapper::getRandomIslamicFlag();
				}
				else if (religion == "mahayana" || religion == "gelugpa" || religion == "theravada" || religion == "sikh" || religion == "hindu" || religion == "jain")
				{
					randomCK2title = mappers::CK2TitleMapper::getRandomIndianFlag();
				}

				if (randomCK2title && (usableFlagTags.find(*randomCK2title) != usableFlagTags.end()))
				{
					LOG(LogLevel::Info) << "Country " << i->first << " (" << i->second->getLocalName() << ") has been given the CK2 flag " << *randomCK2title;
					tagMap[i->first] =* randomCK2title;
					usableFlagTags.erase(*randomCK2title);
					requiredTags.erase(i->first);
				}
			}
		}
	}

	for (std::set<std::string>::iterator i = usableFlagTags.begin(); i != usableFlagTags.end();)
	{
		const std::string& tag = (*i);
		if (tag.length() > 3 && tag[1] != '_') {
			usableFlagTags.erase(i++);
		}
		else {
			++i;
		}
	}

	for (auto country : V2Countries)
	{
		V2Country* overlord = country.second->getColonyOverlord();
		if (NULL == overlord)
			continue;

		std::string name = country.second->getLocalName();
		name = Utils::convertUTF8To8859_15(name);
		transform(name.begin(), name.end(), name.begin(), ::tolower);

		auto colonialtitle = mappers::colonyFlagsetMapper::getFlag(name);
		if (!colonialtitle)
		{
			colonialFail.push_back(country.second);
			continue;
		}

		colonialtitle->setOverlord(overlord->getTag());
		colonialFlagMapping[country.first] = colonialtitle;

		usableFlagTags.erase(colonialtitle->getName());
		requiredTags.erase(country.first);
		mappers::colonyFlagsetMapper::removeFlag(colonialtitle->getName());
	}

	if (colonialFail.size() != 0)
	{
		std::vector<std::string> colonyFlagsKeys = mappers::colonyFlagsetMapper::getNames();

		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(colonyFlagsKeys.begin(), colonyFlagsKeys.end(), g);

		for (std::string key : colonyFlagsKeys)
		{
			auto flag = mappers::colonyFlagsetMapper::getFlag(key);

			if (false == flag->getOverlord().empty())
			{
				continue;
			}

			for (std::vector<V2Country*>::iterator v2c = colonialFail.begin(); v2c != colonialFail.end(); ++v2c)
			{
				bool success = false;
				std::string region = (*v2c)->getColonialRegion();
				if ((region == "") || (flag->getRegion() == region))
				{
					success = true;
					colonialFlagMapping[(*v2c)->getTag()] = flag;
					V2Country* overlord = (*v2c)->getColonyOverlord();
					std::string overlordName = overlord->getTag();
					flag->setOverlord(overlordName);
					LOG(LogLevel::Info) << "Country with tag " << (*v2c)->getTag() << " is now " << key << ", ruled by " << overlordName;

					usableFlagTags.erase(flag->getName());
					requiredTags.erase((*v2c)->getTag());
					colonialFail.erase(v2c);
					break;
				}
			}
		}
	}

	// All the remaining tags now need one of the usable flags.
	size_t mappingsMade = 0;
	for (std::set<std::string>::const_iterator i = requiredTags.cbegin(); i != requiredTags.cend(); ++i)
	{
		const std::string& V2Tag = *i;
		size_t randomTagIndex = std::uniform_int_distribution<size_t>(0, usableFlagTags.size() - 1)(generator);
		std::set<std::string>::const_iterator randomTagIter = usableFlagTags.cbegin();
		advance(randomTagIter, randomTagIndex);
		const std::string& flagTag = *randomTagIter;
		tagMap[V2Tag] = flagTag;
		if (usableFlagTags.size() > requiredTags.size() - tagMap.size())
		{
			usableFlagTags.erase(flagTag);
		}
	}

	for (std::map<std::string, V2Country*>::const_iterator i = V2Countries.begin(); i != V2Countries.end(); ++i)
	{
		auto eu4country = i->second->getSourceCountry();
		if (!eu4country)
			continue;

		std::string tag = i->second->getTag();

		EU4::NationalSymbol nationalColors = eu4country->getNationalColors();
		
		if (nationalColors.isCustomColorsInitialized())
		{
			customFlagMapping[tag] = nationalColors.getCustomColors();
		}
		else if (eu4country->isRevolutionary() && nationalColors.getRevolutionaryColor())
		{
			nationalColors.getCustomColors().setFlagColors(nationalColors.getRevolutionaryColor());
			customFlagMapping[tag] = nationalColors.getCustomColors();
		}

	}
}


inline bool ends_with(std::string const & value, std::string const & ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}


void V2Flags::determineUseableFlags()
{
	std::set<std::string> availableFlags = determineAvailableFlags();

	while (!availableFlags.empty())
	{
		std::string flag = *availableFlags.begin();
		bool hasSuffix = false;
		for (std::vector<std::string>::const_reverse_iterator i = flagFileSuffixes.rbegin(); i != flagFileSuffixes.rend() && !hasSuffix; ++i)
		{
			auto suffix = *i;
			if (ends_with(flag, suffix))
			{
				std::string tag = flag.substr(0, flag.find(suffix));

				// Ensure we have flags for all suffixes of this tag.
				bool haveAllFlags = true;
				for (std::vector<std::string>::const_iterator j = flagFileSuffixes.begin(); j != flagFileSuffixes.end(); ++j)
				{
					std::set<std::string>::iterator findIter = availableFlags.find(tag + *j);
					if (findIter != availableFlags.end())
					{
						availableFlags.erase(findIter);	// no need to consider this flag again
					}
					else
					{
						haveAllFlags = false;
					}
				}
				if (haveAllFlags)
				{
					usableFlagTags.insert(tag);
				}
			}
		}
		availableFlags.erase(flag);	// in case we didn't remove it before, we don't want to consider it again
	}
}


std::set<std::string> V2Flags::determineAvailableFlags()
{
	std::set<std::string> availableFlags;

	const std::vector<std::string> availableFlagFolders = { "flags", theConfiguration.getVic2Path() + "/gfx/flags" };
	for (auto availableFlagFolder: availableFlagFolders)
	{
		Utils::GetAllFilesInFolder(availableFlagFolder, availableFlags);
	}

	return availableFlags;
}


void V2Flags::getRequiredTags(const std::map<std::string, V2Country*>& V2Countries)
{
	for (auto country: V2Countries)
	{
		requiredTags.insert(country.first);
	}
}


void V2Flags::mapTrivialTags()
{
	std::set<std::string> usableFlagTagsRemaining;
	std::set<std::string> requiredTagsRemaining;

	for (auto requiredTag: requiredTags)
	{
		auto flagTag = usableFlagTags.find(requiredTag);
		if (flagTag != usableFlagTags.end())
		{
			tagMap.insert(make_pair(requiredTag, *flagTag));
			usableFlagTags.erase(flagTag);
		}
		else
		{
			requiredTagsRemaining.insert(requiredTag);
		}
	}

	swap(requiredTags, requiredTagsRemaining);
}


void V2Flags::output() const
{
	LOG(LogLevel::Debug) << "Creating flags";
	createOutputFolders();
	copyFlags();
	createCustomFlags();
	createColonialFlags();
}


void V2Flags::createOutputFolders() const
{
	if (!Utils::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/gfx"))
	{
		LOG(LogLevel::Error) << "Could not create output/" << theConfiguration.getOutputName() << "/gfx";
		exit(-1);
	}
	if (!Utils::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/gfx/flags"))
	{
		LOG(LogLevel::Error) << "Could not create output/" << theConfiguration.getOutputName() << "/gfx/flags";
		exit(-1);
	}
}


void V2Flags::copyFlags() const
{
	const std::vector<std::string> availableFlagFolders = { "flags", theConfiguration.getVic2Path() + "/gfx/flags" };
	for (auto tagMapping: tagMap)
	{
		const std::string& V2Tag = tagMapping.first;
		const std::string& flagTag = tagMapping.second;
		for (std::vector<std::string>::const_iterator i = flagFileSuffixes.begin(); i != flagFileSuffixes.end(); ++i)
		{
			const std::string& suffix = *i;
			bool flagFileFound = false;
			for (std::vector<std::string>::const_iterator j = availableFlagFolders.begin(); j != availableFlagFolders.end() && !flagFileFound; ++j)
			{
				const std::string& folderPath = *j;
				std::string sourceFlagPath = folderPath + '/' + flagTag + suffix;
				flagFileFound = Utils::DoesFileExist(sourceFlagPath);
				if (flagFileFound)
				{
					std::string destFlagPath = "output/" + theConfiguration.getOutputName() + "/gfx/flags/" + V2Tag + suffix;
					Utils::TryCopyFile(sourceFlagPath, destFlagPath);
				}
			}
		}
	}
}


void V2Flags::createCustomFlags() const
{
	std::string baseFlagFolder = "flags";

	LOG(LogLevel::Info) << "Parsing EU4 flag colours";
	std::string colorFileStr = theConfiguration.getEU4Path() + "/common/custom_country_colors/00_custom_country_colors.txt";
	std::ifstream colorFile(colorFileStr);
	if (colorFile.fail())
	{
		std::range_error exception("Could not open flag colors.");
		throw exception;
	}

	mappers::FlagColorMapper flagColorMapper(colorFile);
	colorFile.close();

	for (auto cflag : customFlagMapping)
	{
		std::string V2Tag = cflag.first;
		EU4::CustomColors customColors = cflag.second;

		int baseFlag = customColors.getCustomColors().flag;
		std::string baseFlagStr = std::to_string(baseFlag);
		int emblem = customColors.getCustomColors().symbolIndex;
		commonItems::Color flagColor = customColors.getCustomColors().flagColors;
		int r, g, b = 0;
		flagColor.GetRGB(r, g, b);

		int colourcount = flagColorMapper.getNumFlagColors();

		if (r > colourcount || g > colourcount || b > colourcount)
		{
			LOG(LogLevel::Error) << V2Tag << "'s flag has some missing colours.";
			continue;
		}

		for (int i = 0; i<5; i++)
		{
			if (baseFlag == 0)
				baseFlagStr = "tricolor";

			if ((baseFlag == 0) && i != 0 && i != 4)
				continue;

			const std::string& suffix = flagFileSuffixes[i];
			bool flagFileFound = false;
			std::string folderPath = baseFlagFolder;
			std::string sourceEmblemPath = folderPath + "/CustomEmblems/" + std::to_string(emblem) + suffix;

			std::string sourceFlagPath = folderPath + "/CustomBases/" + baseFlagStr + ".tga";

			flagFileFound = (Utils::DoesFileExist(sourceFlagPath) && Utils::DoesFileExist(sourceEmblemPath));
			if (flagFileFound)
			{
				std::string destFlagPath = "output/" + theConfiguration.getOutputName() + "/gfx/flags/" + V2Tag + suffix;

				std::optional<commonItems::Color> rColor = flagColorMapper.getFlagColorByIndex(r);
				std::optional<commonItems::Color> gColor = flagColorMapper.getFlagColorByIndex(g);
				std::optional<commonItems::Color> bColor = flagColorMapper.getFlagColorByIndex(b);
				if (!rColor) rColor = commonItems::Color();
				if (!gColor) gColor = commonItems::Color();
				if (!bColor) bColor = commonItems::Color();

				CreateCustomFlag(*rColor, *gColor, *bColor, sourceEmblemPath, sourceFlagPath, destFlagPath);
			}
			else
			{
				if (!Utils::DoesFileExist(sourceFlagPath))
				{
					LOG(LogLevel::Error) << "Could not find " << sourceFlagPath;
					std::string err = "Could not find " + sourceFlagPath;
					std::runtime_error exception(err);
					throw exception;
				}
				else
				{
					LOG(LogLevel::Error) << "Could not find " << sourceEmblemPath;
					std::string err = "Could not find " + sourceEmblemPath;
					std::runtime_error exception(err);
					throw exception;
				}
			}
		}
	}
}


void V2Flags::createColonialFlags() const
{
	// I really shouldn't be hardcoding this...
	std::set<std::string> UniqueColonialFlags{ "alyeska", "newholland", "acadia", "kanata", "novascotia", "novahollandia", "vinland", "newspain" };

	//typedef map<string, shared_ptr<colonyFlag> > V2TagToColonyFlagMap; // tag, {base,overlordtag}
	for (auto i : colonialFlagMapping)
	{
		std::string V2Tag = i.first;
		std::string baseFlag = i.second->getName();
		transform(baseFlag.begin(), baseFlag.end(), baseFlag.begin(), ::tolower);
		baseFlag.erase(remove_if(baseFlag.begin(), baseFlag.end(), [](const char ch) { return !isalpha(ch); }), baseFlag.end());

		std::string overlord = i.second->getOverlord();

		for (int i = 0; i < 5; i++)
		{
			const std::string& suffix = flagFileSuffixes[i];
			bool flagFileFound = false;
			std::string folderPath = "flags";

			if ((i == 0 || i == 3) // monarchy or vanilla
				&& (UniqueColonialFlags.find(baseFlag) == UniqueColonialFlags.end()))
			{
				std::string sourceFlagPath = folderPath + '/' + baseFlag + suffix;

				auto overlordFlag = tagMap.find(overlord);
				if (overlordFlag == tagMap.end())
				{
					LOG(LogLevel::Error) << "No flag exists for overlord " << overlord << ". Cannot create colony flag";
					exit(-1);
				}
				std::string overlordFlagPath = folderPath + '/' + overlordFlag->second + ".tga";
				flagFileFound = (Utils::DoesFileExist(sourceFlagPath) && Utils::DoesFileExist(overlordFlagPath));
				if (flagFileFound)
				{
					std::string destFlagPath = "output/" + theConfiguration.getOutputName() + "/gfx/flags/" + V2Tag + suffix;
					CreateColonialFlag(overlordFlagPath, sourceFlagPath, destFlagPath);
				}
				else
				{
					if (!Utils::DoesFileExist(sourceFlagPath))
					{
						LOG(LogLevel::Error) << "Could not find " << sourceFlagPath;
						exit(-1);
					}
					else
					{
						LOG(LogLevel::Error) << "Could not find " << overlordFlagPath;
						exit(-1);
					}
				}
			}
			else
			{
				std::string sourceFlagPath = folderPath + '/' + baseFlag + suffix;
				flagFileFound = Utils::DoesFileExist(sourceFlagPath);
				if (flagFileFound)
				{
					std::string destFlagPath = "output/" + theConfiguration.getOutputName() + "/gfx/flags/" + V2Tag + suffix;
					Utils::TryCopyFile(sourceFlagPath, destFlagPath);
				}
				else
				{
					LOG(LogLevel::Error) << "Could not find " << sourceFlagPath;
				}
			}
		}
	}
}