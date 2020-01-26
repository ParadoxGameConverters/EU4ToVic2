#include "BuildingPosition.h"
#include "NavalBase.h"
#include "ParserHelpers.h"

mappers::BuildingPosition::BuildingPosition(std::istream& theStream)
{
	registerKeyword("building_position", [this](const std::string& unused, std::istream& theStream)
	{
			const NavalBase potentialBase(theStream);
			if (potentialBase.getFound()) found = true;
	});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
