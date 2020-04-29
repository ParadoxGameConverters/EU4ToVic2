#ifndef CLIMATE_MAPPER_H
#define CLIMATE_MAPPER_H

#include "Parser.h"
#include <map>

namespace mappers
{
	class ClimateMapper: commonItems::parser
	{
	public:
		ClimateMapper();
		explicit ClimateMapper(std::istream& theStream);
		
		[[nodiscard]] const auto& getClimateMap() const { return climateMap; }

	private:
		void registerKeys();
		
		std::map<std::string, std::vector<int>> climateMap;
		bool mild_climate = false;
		bool temperate_climate = false;
		bool harsh_climate = false;
		bool inhospitable_climate = false;
	};
}

#endif // CLIMATE_MAPPER_H
