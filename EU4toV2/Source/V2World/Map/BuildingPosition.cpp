#include "BuildingPosition.h"
#include "NavalBase.h"
#include "ParserHelpers.h"

mappers::BuildingPosition::BuildingPosition(std::istream& theStream)
{
	registerKeyword(std::regex("building_position"), [this](const std::string& unused, std::istream& theStream)
	{
			NavalBase potentialBase(theStream);
			if (potentialBase.getFound()) found = true;
	});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}
