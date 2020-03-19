#include "Flags.h"
#include <algorithm>
#include <chrono>
#include <iterator>
#include <random>
#include <fstream>
#include "../../EU4World/Country/EU4Country.h"
#include "../Country/Country.h"
#include "../../Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "../../Mappers/CK2Titles/CK2TitleMapper.h"
#include "FlagUtils.h"

const std::vector<std::string> V2::Flags::flagFileSuffixes = { ".tga", "_communist.tga", "_fascist.tga", "_monarchy.tga", "_republic.tga" };

void V2::Flags::setV2Tags(const std::map<std::string, std::shared_ptr<Country>>& V2Countries, const mappers::CountryMappings& countryMapper)
{
	tagMap.clear();

	static std::mt19937 generator(static_cast<int>(std::chrono::system_clock::now().time_since_epoch().count()));

	determineUseableFlags();
	getRequiredTags(V2Countries);
	mapTrivialTags();

	std::vector<std::shared_ptr<Country>> colonialFail;
	
	// Get the CK2 and colonial flags.
	for (const auto& country: V2Countries)
	{
		// If this is an eu4 import and we need to do this at all
		if (country.second->getSourceCountry() && requiredTags.count(country.first))
		{
			// Check for ck2 name, if one exists
			auto ck2title = countryMapper.getCK2Title(country.first, country.second->getLocalName(), usableFlagTags);
			// do we have a ready ck2 map?
			if (ck2title && usableFlagTags.count(*ck2title))
			{
				tagMap[country.first] = *ck2title;
				usableFlagTags.erase(*ck2title);
				requiredTags.erase(country.first);
			}
			else // try something patronymic
			{
				if (!mappers::CountryMappings::tagIsAlphaDigitDigit(country.first)) continue;

				std::string religion = country.second->getReligion();
				std::optional<std::string> randomCK2title;

				// Yay hardcoded paths. If I get round to it, I'll point these at religion.txt instead.
				if (religion == "sunni" || religion == "shiite" || religion == "ibadi")
				{
					randomCK2title = countryMapper.getCK2TitleMapper().getRandomIslamicFlag();
				}
				else if (religion == "mahayana" || religion == "gelugpa" || religion == "theravada" || religion == "sikh" || religion == "hindu" || religion == "jain")
				{
					randomCK2title = countryMapper.getCK2TitleMapper().getRandomIndianFlag();
				}

				if (randomCK2title && usableFlagTags.count(*randomCK2title))
				{
					tagMap[country.first] = *randomCK2title;
					usableFlagTags.erase(*randomCK2title);
					requiredTags.erase(country.first);
				}
			}
		}
	}

	// Drop all incoming flag titles with _ in second spot (k_bavaria)
	for (auto flagItr = usableFlagTags.begin(); flagItr != usableFlagTags.end();)
	{
		const std::string& tag = *flagItr;
		if (tag[1] != '_')
		{
			usableFlagTags.erase(flagItr++);
		}
		else
		{
			++flagItr;
		}
	}

	for (const auto& country : V2Countries)
	{
		if (!requiredTags.count(country.first)) continue; // use existing map if one is not required.

		auto overlord = country.second->getColonyOverlord();
		if (overlord.empty()) continue;

		std::string name = country.second->getLocalName();
		name = Utils::convertUTF8ToWin1252(name);
		transform(name.begin(), name.end(), name.begin(), ::tolower);

		auto colonialtitle = colonialFlagsMapper.getFlag(name);
		if (!colonialtitle)
		{
			colonialFail.push_back(country.second);
			continue;
		}

		colonialtitle->setOverlord(overlord);
		colonialFlagMapping[country.first] = *colonialtitle;

		usableFlagTags.erase(colonialtitle->getName());
		requiredTags.erase(country.first);
		colonialFlagsMapper.removeFlag(colonialtitle->getName());
	}

	if (!colonialFail.empty())
	{
		std::vector<std::string> colonyFlagsKeys = colonialFlagsMapper.getNames();

		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(colonyFlagsKeys.begin(), colonyFlagsKeys.end(), g);

		for (const auto& key : colonyFlagsKeys)
		{
			auto flag = colonialFlagsMapper.getFlag(key);

			if (!flag) continue;
			if (!flag->getOverlord().empty()) continue;

			for (auto failCountryItr = colonialFail.begin(); failCountryItr != colonialFail.end(); ++failCountryItr)
			{
				std::string region = (*failCountryItr)->getColonialRegion();
				if (region.empty() || flag->getRegion() == region)
				{
					colonialFlagMapping[(*failCountryItr)->getTag()] = *flag;
					auto overlord = (*failCountryItr)->getColonyOverlord();
					flag->setOverlord(overlord);
					LOG(LogLevel::Info) << "\tA premade flag is available for " << (*failCountryItr)->getTag() << " (" << key << "), ruled by " << overlord;

					usableFlagTags.erase(flag->getName());
					requiredTags.erase((*failCountryItr)->getTag());
					colonialFail.erase(failCountryItr);
					break;
				}
			}
		}
	}

	// All the remaining tags now need one of the usable flags.
	for (const auto& requiredTag: requiredTags)
	{
		size_t randomTagIndex = std::uniform_int_distribution<size_t>(0, usableFlagTags.size() - 1)(generator);
		auto randomTagIter = usableFlagTags.begin();
		advance(randomTagIter, randomTagIndex);
		tagMap[requiredTag] = *randomTagIter;
		if (usableFlagTags.size() > requiredTags.size() - tagMap.size())
		{
			usableFlagTags.erase(*randomTagIter);
		}
	}

	for (const auto& country : V2Countries)
	{
		auto eu4country = country.second->getSourceCountry();
		if (!eu4country)
			continue;

		std::string tag = country.second->getTag();
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

void V2::Flags::determineUseableFlags()
{
	auto availableFlags = determineAvailableFlags();

	while (!availableFlags.empty())
	{
		auto flag = *availableFlags.begin();
		for (auto flagFileSuffixItr = flagFileSuffixes.rbegin(); flagFileSuffixItr != flagFileSuffixes.rend(); ++flagFileSuffixItr)
		{
			auto suffix = *flagFileSuffixItr;
			if (ends_with(flag, suffix))
			{
				auto tag = flag.substr(0, flag.find(suffix));

				// Ensure we have flags for all suffixes of this tag.
				auto haveAllFlags = true;
				for (const auto& flagFileSuffix: flagFileSuffixes)
				{
					auto findIter = availableFlags.find(tag + flagFileSuffix);
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
		availableFlags.erase(flag); // in case we didn't remove it before, we don't want to consider it again
	}
}

std::set<std::string> V2::Flags::determineAvailableFlags()
{
	std::set<std::string> availableFlags;

	const std::vector<std::string> availableFlagFolders = { "flags", theConfiguration.getVic2Path() + "/gfx/flags" };
	for (const auto& availableFlagFolder: availableFlagFolders)
	{
		Utils::GetAllFilesInFolder(availableFlagFolder, availableFlags);
	}

	return availableFlags;
}

void V2::Flags::getRequiredTags(const std::map<std::string, std::shared_ptr<Country>>& V2Countries)
{
	for (const auto& country: V2Countries)
	{
		requiredTags.insert(country.first);
	}
}

void V2::Flags::mapTrivialTags()
{
	std::set<std::string> usableFlagTagsRemaining;
	std::set<std::string> requiredTagsRemaining;

	for (const auto& requiredTag: requiredTags)
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

void V2::Flags::output() const
{
	createOutputFolders();
	copyFlags();
	createCustomFlags();
	createColonialFlags();
}

void V2::Flags::createOutputFolders()
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

void V2::Flags::copyFlags() const
{
	const std::vector<std::string> availableFlagFolders = { "flags", theConfiguration.getVic2Path() + "/gfx/flags" };
	for (const auto& tagMapping: tagMap)
	{
		const auto V2Tag = tagMapping.first;
		const auto flagTag = tagMapping.second;
		for (const auto& flagFileSuffix: flagFileSuffixes)
		{
			auto flagFileFound = false;
			for (auto availableFolderItr = availableFlagFolders.begin(); availableFolderItr != availableFlagFolders.end() && !flagFileFound; ++availableFolderItr)
			{
				const auto sourceFlagPath = *availableFolderItr + '/' + flagTag + flagFileSuffix;
				flagFileFound = Utils::DoesFileExist(sourceFlagPath);
				if (flagFileFound)
				{
					const auto destFlagPath = "output/" + theConfiguration.getOutputName() + "/gfx/flags/" + V2Tag + flagFileSuffix;
					Utils::TryCopyFile(sourceFlagPath, destFlagPath);
				}
			}
		}
	}
}

void V2::Flags::createCustomFlags() const
{
	std::string baseFlagFolder = "flags";

	for (const auto& cflag : customFlagMapping)
	{
		const auto V2Tag = cflag.first;
		const auto customColors = cflag.second;

		auto baseFlag = customColors.getCustomColors().flag;
		auto baseFlagStr = std::to_string(baseFlag);
		auto emblem = customColors.getCustomColors().symbolIndex;
		auto flagColor = customColors.getCustomColors().flagColors;
		int r, g, b = 0;
		flagColor.GetRGB(r, g, b);

		int colourcount = flagColorMapper.getNumFlagColors();

		if (r > colourcount || g > colourcount || b > colourcount)
		{
			LOG(LogLevel::Error) << V2Tag << "'s flag has some missing colours.";
			continue;
		}

		// We have 5 flags for every tag
		for (auto i = 0; i < 5; i++)
		{
			if (baseFlag == 0) baseFlagStr = "tricolor";

			// For custom/revolutionary flags we only create the fascist, absolutist and communist
			if (baseFlag == 0 && i != 0 && i != 4)
				continue;

			const auto& suffix = flagFileSuffixes[i];
			auto flagFileFound = false;
			auto sourceEmblemPath = baseFlagFolder + "/CustomEmblems/" + std::to_string(emblem) + suffix;
			auto sourceFlagPath = baseFlagFolder + "/CustomBases/" + baseFlagStr + ".tga";

			flagFileFound = Utils::DoesFileExist(sourceFlagPath) && Utils::DoesFileExist(sourceEmblemPath);
			if (flagFileFound)
			{
				auto destFlagPath = "output/" + theConfiguration.getOutputName() + "/gfx/flags/" + V2Tag + suffix;

				auto rColor = flagColorMapper.getFlagColorByIndex(r);
				auto gColor = flagColorMapper.getFlagColorByIndex(g);
				auto bColor = flagColorMapper.getFlagColorByIndex(b);
				if (!rColor) rColor = commonItems::Color();
				if (!gColor) gColor = commonItems::Color();
				if (!bColor) bColor = commonItems::Color();
				createCustomFlag(*rColor, *gColor, *bColor, sourceEmblemPath, sourceFlagPath, destFlagPath);
			}
			else
			{
				if (!Utils::DoesFileExist(sourceFlagPath)) throw std::runtime_error("Could not find " + sourceFlagPath);
				throw std::runtime_error("Could not find " + sourceEmblemPath);
			}
		}
	}
}

void V2::Flags::createColonialFlags() const
{
	// I really shouldn't be hardcoding this...
	std::set<std::string> UniqueColonialFlags{ "alyeska", "newholland", "acadia", "kanata", "novascotia", "novahollandia", "vinland", "newspain" };

	for (const auto& colonialFlag : colonialFlagMapping)
	{
		auto V2Tag = colonialFlag.first;
		auto baseFlag = colonialFlag.second.getName();
		transform(baseFlag.begin(), baseFlag.end(), baseFlag.begin(), ::tolower);
		baseFlag.erase(remove_if(baseFlag.begin(), baseFlag.end(), [](const char ch) { return !isalpha(ch); }), baseFlag.end());

		auto overlord = colonialFlag.second.getOverlord();

		for (auto i = 0; i < 5; i++)
		{
			const auto& suffix = flagFileSuffixes[i];
			const auto folderPath = "flags";
			bool flagFileFound;

			if ((i == 0 || i == 3) // monarchy or vanilla
				&& UniqueColonialFlags.find(baseFlag) == UniqueColonialFlags.end())
			{
				auto sourceFlagPath = folderPath + std::string("/") + baseFlag + suffix;

				auto overlordFlag = tagMap.find(overlord);
				if (overlordFlag == tagMap.end()) throw std::runtime_error("No flag exists for " + V2Tag + "'s overlord " + overlord + ". Cannot create colony flag.");

				auto overlordFlagPath = folderPath + std::string("/") + overlordFlag->second + ".tga";
				flagFileFound = Utils::DoesFileExist(sourceFlagPath) && Utils::DoesFileExist(overlordFlagPath);
				if (flagFileFound)
				{
					auto destFlagPath = "output/" + theConfiguration.getOutputName() + "/gfx/flags/" + V2Tag + suffix;
					createColonialFlag(overlordFlagPath, sourceFlagPath, destFlagPath);
				}
				else
				{
					if (!Utils::DoesFileExist(sourceFlagPath)) throw std::runtime_error("Could not find " + sourceFlagPath);
					throw std::runtime_error("Could not find " + overlordFlagPath);
				}
			}
			else
			{
				auto sourceFlagPath = folderPath + std::string("/") + baseFlag + suffix;
				flagFileFound = Utils::DoesFileExist(sourceFlagPath);
				if (flagFileFound)
				{
					auto destFlagPath = "output/" + theConfiguration.getOutputName() + "/gfx/flags/" + V2Tag + suffix;
					Utils::TryCopyFile(sourceFlagPath, destFlagPath);
				}
				else
				{
					throw std::runtime_error("Could not find " + sourceFlagPath);
				}
			}
		}
	}
}