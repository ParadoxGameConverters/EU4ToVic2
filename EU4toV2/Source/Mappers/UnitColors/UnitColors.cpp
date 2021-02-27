#include "UnitColors.h"
#include "CommonRegexes.h"
#include "Configuration.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

mappers::UnitColors::UnitColors()
{
	registerKeys();
	parseFile("./blankMod/output/common/country_colors.txt");
	if (const auto& mod = theConfiguration.getVic2ModName();
		 !mod.empty() && commonItems::DoesFileExist(theConfiguration.getVic2ModPath() + "/" + mod + "/common/country_colors.txt"))
	{
		parseFile(theConfiguration.getVic2ModPath() + "/" + mod + "/common/country_colors.txt");
	}
	clearRegisteredKeywords();
}

mappers::UnitColors::UnitColors(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::UnitColors::registerKeys()
{
	registerRegex("[A-Z]{3}", [this](const std::string& tag, std::istream& theStream) {
		unitColorsMap.insert(make_pair(tag, commonItems::stringOfItem(theStream).getString()));
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}