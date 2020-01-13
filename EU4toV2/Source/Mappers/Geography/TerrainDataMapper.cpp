#include "TerrainDataMapper.h"
#include "ParserHelpers.h"

mappers::TerrainDataMapper::TerrainDataMapper()
{
	registerKeys();
	parseFile("terrainData.txt");
	clearRegisteredKeywords();
}

mappers::TerrainDataMapper::TerrainDataMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::TerrainDataMapper::registerKeys()
{
	registerRegex("[0-9]+", [this](const std::string& provID, std::istream& theStream)
		{
			commonItems::singleString terrainStr(theStream);
			terrainMap.insert(std::make_pair(stoi(provID), terrainStr.getString()));
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}