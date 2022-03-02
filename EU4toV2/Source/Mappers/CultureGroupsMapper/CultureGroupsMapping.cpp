#include "CultureGroupsMapping.h"
#include "../../Configuration.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

mappers::CultureGroupsMapping::CultureGroupsMapping(std::istream& theStream)
{
	registerKeys(theConfiguration.isHpmEnabled());
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::CultureGroupsMapping::registerKeys(bool isHPMEnabled)
{
	registerKeyword("v2", [this](std::istream& theStream) {
		v2 = commonItems::getString(theStream);
	});
	registerKeyword("hpm", [this, isHPMEnabled](std::istream& theStream) {
		hpm = commonItems::getString(theStream);
		if (isHPMEnabled)
			v2 = *hpm;
	});
	registerKeyword("eu4", [this](std::istream& theStream) {
		eu4s.insert(commonItems::getString(theStream));
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
