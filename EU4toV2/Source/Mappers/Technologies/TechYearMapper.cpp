#include "TechYearMapper.h"
#include "CommonRegexes.h"
#include "Configuration.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include "Technology.h"

mappers::TechYearMapper::TechYearMapper()
{
	registerKeys();
	parseFile("configurables" + theConfiguration.getVic2ModName() + "/technologies.txt");
	clearRegisteredKeywords();
}

void mappers::TechYearMapper::registerKeys()
{
	registerRegex("[0-9]+", [this](const std::string& year, std::istream& theStream) {
		commonItems::stringsOfItemNames yearStr(theStream);
		for (const auto& tech: yearStr.getStrings())
		{
			techYearMap[tech] = year;
		}
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}