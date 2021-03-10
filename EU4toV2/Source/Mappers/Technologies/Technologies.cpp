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
		categories.insert(make_pair(file, getTechsInCategory()));
	}
	clearRegisteredKeywords();

	setTechLevels();
	makeIdNameMap();
	updateTechs();
}

void mappers::Technologies::registerKeys(const std::string& filename)
{
	techsInCategory.clear();
	registerRegex("[A-Za-z0-9_]+", [this](const std::string& techName, std::istream& theStream) {
		Technology newTech(theStream);
		newTech.setTechName(techName);
		techsInCategory.push_back(newTech);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void mappers::Technologies::updateTechs()
{
	const auto& techYearMap = techYearMapper.getMap();
	for (auto& [unused, technologies]: categories)
	{
		for (auto& tech: technologies)
		{
			if (techYearMap.contains(tech.getName()))
				tech.setYear(techYearMap.at(tech.getName()));
		}
	}
}

void mappers::Technologies::setTechLevels()
{
	for (auto& [unused, technologies]: categories)
	{
		std::string group;
		int i = 1;

		for (auto& tech: technologies)
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

void mappers::Technologies::makeIdNameMap()
{
	for (const auto& [unused, technologies]: categories)
	{
		for (const auto& tech: technologies)
		{
			const auto& id = tech.getArea() + "_" + std::to_string(tech.getLevel());
			idNameMap[id] = tech.getName();
		}
	}
}