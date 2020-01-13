#include "CK2TitleMapper.h"
#include "TitleMapping.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

mappers::CK2TitleMapper::CK2TitleMapper()
{
	LOG(LogLevel::Info) << "Getting CK2 titles";
	registerKeys();
	parseFile("configurables/ck2_title_map.txt");
	clearRegisteredKeywords();
}

mappers::CK2TitleMapper::CK2TitleMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::CK2TitleMapper::registerKeys()
{
	registerKeyword("link", [this](const std::string& unused, std::istream& theStream)
		{
			TitleMapping newMapping(theStream);
			if (newMapping.hasIslamicRegion())
			{
				islamicFlags.push_back(newMapping.getID());
			}
			else if (newMapping.hasIndianRegion())
			{
				indianFlags.push_back(newMapping.getID());
			}

			titleMap[newMapping.getName()] = newMapping.getID();
			titles.insert(newMapping.getID());
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);
}

std::optional<std::string> mappers::CK2TitleMapper::getTitle(std::string name) const
{
	auto mapping = titleMap.find(name);
	if (mapping != titleMap.end()) return mapping->second;
	return std::nullopt;
}

bool mappers::CK2TitleMapper::doesTitleExist(const std::string& title) const
{
	return (titles.count(title) > 0);
}

std::optional<std::string> mappers::CK2TitleMapper::getRandomIslamicFlag() const
{
	if (islamicFlags.size() > 0)
	{
		std::vector<std::string> randomFlags;
		std::sample(islamicFlags.begin(), islamicFlags.end(), std::inserter(randomFlags, randomFlags.begin()), 1, std::mt19937{ std::random_device{}() });
		return *randomFlags.begin();
	}
	else
	{
		return std::nullopt;
	}
}

std::optional<std::string> mappers::CK2TitleMapper::getRandomIndianFlag() const
{
	if (indianFlags.size() > 0)
	{
		std::vector<std::string> randomFlags;
		std::sample(indianFlags.begin(), indianFlags.end(), std::inserter(randomFlags, randomFlags.begin()), 1, std::mt19937{ std::random_device{}() });
		return *randomFlags.begin();
	}
	else
	{
		return std::nullopt;
	}
}