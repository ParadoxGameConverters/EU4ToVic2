#ifndef TERRAIN_DATA_MAPPER_H
#define TERRAIN_DATA_MAPPER_H

#include "Parser.h"
#include <map>
#include <optional>

namespace mappers
{
	class TerrainDataMapper: commonItems::parser
	{
	public:
		TerrainDataMapper();
		explicit TerrainDataMapper(std::istream& theStream);
		
		[[nodiscard]] const auto& getTerrainMap() const { return terrainMap; }
		[[nodiscard]] std::optional<std::string> getTerrainForID(int provinceID) const;

	private:
		void registerKeys();
		
		std::map<int, std::string> terrainMap;
	};
}

#endif // TERRAIN_DATA_MAPPER_H
