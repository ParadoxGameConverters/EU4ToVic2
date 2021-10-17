#include "VNColonialMapper.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

mappers::VNColonialMapper::VNColonialMapper()
{
	registerKeys();
	parseFile("configurables/vn_colonial.txt");
	clearRegisteredKeywords();
}

mappers::VNColonialMapper::VNColonialMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::VNColonialMapper::registerKeys()
{
	registerKeyword("link", [this](std::istream& theStream) {
		VNColonialMapping theMapping(theStream);
		VNColonies.emplace_back(theMapping);
		allColonialProvinces.insert(theMapping.getProvinces().begin(), theMapping.getProvinces().end());
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
