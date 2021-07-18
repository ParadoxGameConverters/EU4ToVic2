#include "BuildingPosition.h"
#include "CommonRegexes.h"
#include "NavalBase.h"
#include "ParserHelpers.h"

mappers::BuildingPosition::BuildingPosition(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::BuildingPosition::registerKeys()
{
	registerKeyword("building_position", [this](const std::string& unused, std::istream& theStream) {
		const NavalBase potentialBase(theStream);
		found = potentialBase.isFound();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
