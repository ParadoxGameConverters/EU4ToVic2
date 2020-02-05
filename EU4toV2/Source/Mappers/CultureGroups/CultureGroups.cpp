#include "CultureGroups.h"
#include "../../Configuration.h"
#include "OSCompatibilityLayer.h"
#include <set>
#include "Log.h"
#include "../CultureMapper/CultureMapper.h"
#include "../../EU4World/World.h"

void mappers::CultureGroups::initForEU4()
{
	LOG(LogLevel::Info) << "Parsing Cultures and Culture Groups";
	registerKeys();

	std::set<std::string> cultureFiles;
	Utils::GetAllFilesInFolder(theConfiguration.getEU4Path() + "/common/cultures/", cultureFiles);
	for (const auto& cultureFile : cultureFiles)
	{
		parseFile(theConfiguration.getEU4Path() + "/common/cultures/" + cultureFile);
	}
	for (const auto& itr: theConfiguration.getEU4Mods())
	{
		std::set<std::string> moreCultureFiles;
		Utils::GetAllFilesInFolder(itr + "/common/cultures/", moreCultureFiles);
		for (const auto& cultureFile: moreCultureFiles)
		{
			parseFile(itr + "/common/cultures/" + cultureFile);
		}
	}
	clearRegisteredKeywords();
}

void mappers::CultureGroups::initForV2()
{
	LOG(LogLevel::Info) << "Parsing V2 Cultures and Culture Groups";
	registerKeys();
	parseFile("configurables/culture_definitions.txt");
	clearRegisteredKeywords();
}

mappers::CultureGroups::CultureGroups(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::CultureGroups::registerKeys()
{
	registerRegex("\\w+", [this](const std::string& cultureGroupName, std::istream& theStream)
		{
			std::vector<Culture> cultures;
			CultureGroup newGroup(cultureGroupName, theStream);
			cultureGroupsMap.insert(std::make_pair(cultureGroupName, newGroup));
		});
}

std::optional<mappers::CultureGroup> mappers::CultureGroups::getGroupForCulture(const std::string& cultureName) const
{
	for (const auto& cultureGroupItr: cultureGroupsMap) 
		if (cultureGroupItr.second.containsCulture(cultureName)) 
			return cultureGroupItr.second;
	return std::nullopt;
}

mappers::CultureGroup* mappers::CultureGroups::retrieveCultureGroup(const std::string& cultureName)
{
	for (auto& cultureGroupItr: cultureGroupsMap) 
		if (cultureGroupItr.second.containsCulture(cultureName)) 
			return &cultureGroupItr.second;
	return nullptr;
}

std::map<std::string, mappers::Culture> mappers::CultureGroups::getCulturesInGroup(const std::string& groupName) const
{
	const auto& mapping = cultureGroupsMap.find(groupName);
	if (mapping != cultureGroupsMap.end()) return mapping->second.getCultures();
	return std::map<std::string, Culture>();
}

mappers::CultureGroup mappers::CultureGroups::getCultureGroup(const std::string& cultureGroupName) const
{
	const auto& cultureGroupItr = cultureGroupsMap.find(cultureGroupName);
	if (cultureGroupItr != cultureGroupsMap.end()) return cultureGroupItr->second;
	return CultureGroup();
}

void mappers::CultureGroups::importNeoCultures(const EU4::World& sourceWorld, const CultureMapper& cultureMapper)
{
	const auto& eu4CultureGroupsMapper = sourceWorld.getCultureGroupsMapper();
	for (const auto& eu4CultureGroupIter: eu4CultureGroupsMapper.getCultureGroupsMap())
	{
		for (const auto& eu4CultureIter: eu4CultureGroupIter.second.getCultures())
		{
			// Hello random eu4 culture. Are you a neoculture?
			if (!eu4CultureIter.second.getNeoCulture()) continue;

			// Let's find out what V2 culture group this one is supposed to belong to.
			const auto& origeu4CultureName = eu4CultureIter.second.getOriginalCulture();
			const auto& destV2cultureName = cultureMapper.cultureMatch(sourceWorld.getRegions(), origeu4CultureName, "", 0, "");
			if (!destV2cultureName)
			{
				// yeah let's not go there.
				Log(LogLevel::Warning) << "Unable to locate culture mapping for EU4 culture: " << origeu4CultureName << ". This will end in tears.";
				continue; 
			}
			const auto& destV2cultureGroup = retrieveCultureGroup(*destV2cultureName);
			if (!destV2cultureGroup) 
			{
				// let's not go there either.
				Log(LogLevel::Warning) << "Unable to locate culture group for V2 culture: " << *destV2cultureName << ". This will end in blood.";
				continue; 
			}

			// Now transmogrify eu4 culture definitions into V2 culture definitions.
			auto v2Culture = eu4CultureIter.second;
			v2Culture.transmogrify();

			// and file under appropriate group.
			destV2cultureGroup->addCulture(eu4CultureIter.first, v2Culture, eu4CultureIter.first);
		}
	}
}
