#ifndef CLIMATE_MAPPER_H
#define CLIMATE_MAPPER_H

#include "newParser.h"
#include <map>

namespace mappers
{
	class ClimateMapper: commonItems::parser
	{
	public:
		ClimateMapper();
		std::map<std::string, std::vector<int>> getClimateMap() const { return climateMap; }

	private:
		std::map<std::string, std::vector<int>> climateMap;
		bool mild_climate = false;
		bool temperate_climate = false;
		bool harsh_climate = false;
		bool inhospitable_climate = false;
	};
}

#endif // COLONIAL_TAG_H
