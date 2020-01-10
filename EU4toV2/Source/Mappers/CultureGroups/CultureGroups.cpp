#include "CultureGroups.h"
#include "../../Configuration.h"
#include "OSCompatibilityLayer.h"
#include <set>

mappers::CultureGroups::CultureGroups()
{
	registerKeys();
	std::set<std::string> cultureFiles;
	Utils::GetAllFilesInFolder(theConfiguration.getEU4Path() + "/common/cultures/", cultureFiles);
	for (auto cultureFile : cultureFiles)
	{
		parseFile(theConfiguration.getEU4Path() + "/common/cultures/" + cultureFile);
	}

	for (auto mod: theConfiguration.getEU4Mods())
	{
		std::set<std::string> cultureFiles;
		Utils::GetAllFilesInFolder(mod + "/common/cultures/", cultureFiles);
		for (auto cultureFile: cultureFiles)
		{
			parseFile(mod + "/common/cultures/" + cultureFile);
		}
	}
}

void mappers::CultureGroups::registerKeys()
{
	registerKeyword(std::regex("\\w+"), [this](const std::string& cultureGroupName, std::istream& theStream)
		{
			std::vector<Culture> cultures;
			CultureGroup newGroup(cultureGroupName, theStream);
			for (auto culture: newGroup.getCultures())
			{
				cultureToGroupMap.insert(make_pair(culture.first, newGroup));
				cultures.push_back(culture.second);
			}

			auto itr = groupToCulturesMap.find(cultureGroupName);
			if (itr != groupToCulturesMap.end())
			{
				auto oldCultures = itr->second;
				for (auto oldCulture: oldCultures)
				{
					cultures.push_back(oldCulture);
				}
			}
			groupToCulturesMap[cultureGroupName] = cultures;
		});
}

std::optional<mappers::CultureGroup> mappers::CultureGroups::getCulturalGroup(const std::string& culture)
{
	auto mapping = cultureToGroupMap.find(culture);
	if (mapping != cultureToGroupMap.end()) return mapping->second;
	return std::nullopt;
}

const std::vector<mappers::Culture> mappers::CultureGroups::getCulturesInGroup(const std::string& group)
{
	auto mapping = groupToCulturesMap.find(group);
	if (mapping != groupToCulturesMap.end()) return mapping->second;
	std::vector<Culture> empty;
	return empty;
}