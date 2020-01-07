#ifndef BUILDINGS_H
#define BUILDINGS_H

#include "newParser.h"
#include "Building.h"
#include <map>
#include <optional>

namespace EU4
{
	class Buildings: commonItems::parser
	{
	public:
		Buildings(std::istream& theStream);
		std::optional<Building> getBuilding(const std::string& buildingName) const;

	private:
		std::map<std::string, Building> buildings;
	};
}

#endif // BUILDINGS_H