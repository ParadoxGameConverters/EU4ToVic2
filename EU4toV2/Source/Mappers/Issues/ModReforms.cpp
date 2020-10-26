#include "ModReforms.h"
#include "CommonRegexes.h"
#include "Configuration.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

mappers::ModReforms::ModReforms()
{
	registerKeys();
	parseFile("configurables/HPM/reforms.txt");
	clearRegisteredKeywords();
}

mappers::ModReforms::ModReforms(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::ModReforms::registerKeys()
{
	registerKeyword("unciv", [this](const std::string& unused, std::istream& theStream) {
		ModReforms uncivReforms(theStream);
		uncivs = uncivReforms.getReforms();
	});
	registerRegex(commonItems::catchallRegex, [this](const std::string& reform, std::istream& theStream) {
		reforms.insert(make_pair(reform, commonItems::getString(theStream)));
	});
}