#ifndef SUPERREGIONS_H
#define SUPERREGIONS_H

#include "newParser.h"
#include <map>

namespace EU4
{
	class SuperRegions: commonItems::parser
	{
	public:
		SuperRegions() = default;
		SuperRegions(std::istream& theStream);
		const std::map<std::string, std::vector<std::string>>& getSuperRegions() const { return superRegionMap; }

	private:
		std::map<std::string, std::vector<std::string>> superRegionMap;
	};
}

#endif // SUPERREGIONS_H
