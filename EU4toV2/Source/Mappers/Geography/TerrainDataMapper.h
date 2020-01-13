#ifndef TERRAIN_DATA_MAPPER_H
#define TERRAIN_DATA_MAPPER_H

#include "newParser.h"
#include <map>

namespace mappers
{
	class TerrainDataMapper: commonItems::parser
	{
	public:
		TerrainDataMapper();
		std::map<int, std::string> getTerrainMap() const { return terrainMap; }

	private:
		std::map<int, std::string> terrainMap;
	};
}

#endif // TERRAIN_DATA_MAPPER_H
