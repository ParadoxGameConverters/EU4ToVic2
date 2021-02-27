#include "ModReforms.h"
#include "CommonRegexes.h"
#include "Configuration.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

mappers::ModReforms::ModReforms()
{
	if (commonItems::DoesFileExist("configurables/" + theConfiguration.getVic2ModName() + "/reforms.txt"))
	{
		registerKeys();
		parseFile("configurables/" + theConfiguration.getVic2ModName() + "/reforms.txt");
		clearRegisteredKeywords();
	}
}

mappers::ModReforms::ModReforms(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::ModReforms::registerKeys()
{
	registerRegex("[a-zA-Z_0-9]+", [this](const std::string& reform, std::istream& theStream) {
		const commonItems::singleString positionStr(theStream);
		reforms.insert(make_pair(reform, positionStr.getString()));
	});
	registerKeyword("unciv", [this](const std::string& unused, std::istream& theStream) {
		ModReforms uncivReforms(theStream);
		uncivs = uncivReforms.getReforms();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}