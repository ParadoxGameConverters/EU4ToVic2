#include "MapProvince.h"
#include "BuildingPosition.h"
#include "ParserHelpers.h"
#include "../../Configuration.h"

mappers::MapProvince::MapProvince()
{
	registerKeyword(std::regex("[0-9]+"), [this](const std::string& provinceID, std::istream& theStream)
		{
			mappers::BuildingPosition possibleNaval(theStream);
			if (possibleNaval.getFound()) navalProvinces.insert(stoi(provinceID));
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseFile(theConfiguration.getVic2Path() + "/map/positions.txt");
}
