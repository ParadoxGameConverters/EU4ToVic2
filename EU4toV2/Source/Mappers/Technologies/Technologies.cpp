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
}

void mappers::Technologies::registerKeys(const std::string& filename)
{
	technologies.clear();
	registerRegex("[a-z_]+", [this](const std::string& techName, std::istream& theStream) {
		Technology newTech(theStream, techYearMapper);
		newTech.setTechName(techName);
		technologies.push_back(newTech);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}