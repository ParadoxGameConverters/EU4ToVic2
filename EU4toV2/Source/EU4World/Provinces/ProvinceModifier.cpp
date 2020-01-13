#include "ProvinceModifier.h"
#include "ParserHelpers.h"

EU4::ProvinceModifier::ProvinceModifier(std::istream& theStream)
{
	registerKeyword("modifier", [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleString modifierString(theStream);
		modifier = modifierString.getString();
	});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}