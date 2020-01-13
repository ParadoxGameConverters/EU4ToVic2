#include "TerrainDataMapper.h"
#include "ParserHelpers.h"

mappers::TerrainDataMapper::TerrainDataMapper()
{
	registerKeyword(std::regex("[0-9]+"), [this](const std::string& provID, std::istream& theStream)
	{
			commonItems::singleString terrainStr(theStream);
			terrainMap.insert(std::make_pair(stoi(provID), terrainStr.getString()));
	});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseFile("terrainData.txt");
}
