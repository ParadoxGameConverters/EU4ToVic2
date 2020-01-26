#include "TerrainDataMapper.h"
#include "ParserHelpers.h"
#include "Log.h"

mappers::TerrainDataMapper::TerrainDataMapper()
{
	LOG(LogLevel::Info) << "Loading Terrain Data.";
	registerKeys();
	parseFile("configurables/terrain_data.txt");
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
			const commonItems::singleString terrainStr(theStream);
			terrainMap.insert(std::make_pair(stoi(provID), terrainStr.getString()));
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}

std::optional<std::string> mappers::TerrainDataMapper::getTerrainForID(const int provinceID) const
{
	const auto& terrainItr = terrainMap.find(provinceID);
	if (terrainItr != terrainMap.end()) return terrainItr->second;
	return std::nullopt;
}