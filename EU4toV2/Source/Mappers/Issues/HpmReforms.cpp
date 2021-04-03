#include "HpmReforms.h"
#include "CommonRegexes.h"
#include "Configuration.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

mappers::HpmReforms::HpmReforms()
{
	registerKeys();
	parseFile("configurables/HPM/reforms.txt");
	clearRegisteredKeywords();
}

mappers::HpmReforms::HpmReforms(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::HpmReforms::registerKeys()
{
	registerKeyword("unciv", [this](const std::string& unused, std::istream& theStream) {
		HpmReforms uncivReforms(theStream);
		uncivs = uncivReforms.getReforms();
	});
	registerRegex(commonItems::catchallRegex, [this](const std::string& reform, std::istream& theStream) {
		reforms.insert(make_pair(reform, commonItems::getString(theStream)));
	});
}