#include "ProvinceModifier.h"
#include "ParserHelpers.h"

EU4::ProvinceModifier::ProvinceModifier(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU4::ProvinceModifier::registerKeys()
{
	registerKeyword("modifier", [this](const std::string& unused, std::istream& theStream) {
		modifier = commonItems::singleString(theStream).getString();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
