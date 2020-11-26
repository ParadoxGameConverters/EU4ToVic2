#include "CulturalUnion.h"
#include "ParserHelpers.h"

mappers::CulturalUnion::CulturalUnion(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::CulturalUnion::registerKeys()
{
	registerKeyword("culture", [this](const std::string& unused, std::istream& theStream) {
		culture = commonItems::singleString(theStream).getString();
	});
	registerKeyword("tag", [this](const std::string& unused, std::istream& theStream) {
		tags.emplace_back(commonItems::singleString(theStream).getString());
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
