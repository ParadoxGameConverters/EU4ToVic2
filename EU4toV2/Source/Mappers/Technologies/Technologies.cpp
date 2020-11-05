#include "Technologies.h"
#include "CommonRegexes.h"
#include "Configuration.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

mappers::Technologies::Technologies()
{
	const auto& techFiles = commonItems::GetAllFilesInFolder(theConfiguration.getVic2Path() + "/technologies");
	for (const auto& file: techFiles)
	{
		registerKeys(file);
		parseFile(theConfiguration.getVic2Path() + "/technologies/" + file);
		categories.insert(make_pair(file, getTechnologies()));
	}
	clearRegisteredKeywords();

	setTechLevels();
	updateTechs();
}

void mappers::Technologies::registerKeys(const std::string& filename)
{
	technologies.clear();
	registerRegex("[A-Za-z0-9_]+", [this](const std::string& techName, std::istream& theStream) {
		Technology newTech(theStream);
		newTech.setTechName(techName);
		technologies.push_back(newTech);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void mappers::Technologies::updateTechs()
{
	const auto& techYearMap = techYearMapper.getMap();
	for (auto& category: categories)
	{
		for (auto& tech: category.second)
		{
			if (const auto& mapItr = techYearMap.find(tech.getName()); mapItr != techYearMap.end())
				tech.setYear(mapItr->second);
		}
	}
}

void mappers::Technologies::setTechLevels()
{
	for (auto& category: categories)
	{
		std::string group;
		int i = 1;

		for (auto& tech: category.second)
		{
			if (tech.getArea() == group)
				i++;
			else
			{
				group = tech.getArea();
				i = 1;
			}
			tech.setLevel(i);
		}
	}
}