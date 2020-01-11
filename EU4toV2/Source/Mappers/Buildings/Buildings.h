#ifndef BUILDINGS_H
#define BUILDINGS_H

#include "newParser.h"
#include "Building.h"
#include <map>
#include <optional>

namespace mappers
{
	class Buildings: commonItems::parser
	{
	public:
		Buildings();
		Buildings(std::istream& theStream);
		std::optional<Building> getBuilding(const std::string& buildingName) const;
		void registerKeys();

	private:
		std::map<std::string, Building> buildings;
	};
}

#endif // BUILDINGS_H