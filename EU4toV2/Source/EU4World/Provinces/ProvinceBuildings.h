#ifndef PROVINCE_BUILDINGS_H
#define PROVINCE_BUILDINGS_H

#include "newParser.h"
#include <set>

namespace EU4
{
	class ProvinceBuildings: commonItems::parser
	{
		public:
			ProvinceBuildings() = default;
			ProvinceBuildings(std::istream& theStream);
			bool hasBuilding(const std::string& building) const { return buildings.count(building) > 0; }
			const std::set<std::string>& getBuildings() const { return buildings; }

		private:
			std::set<std::string> buildings;
	};
}

#endif // PROVINCE_BUILDINGS_H