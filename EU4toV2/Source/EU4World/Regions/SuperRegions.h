#ifndef SUPER_REGIONS_H
#define SUPER_REGIONS_H

#include "newParser.h"
#include <map>

namespace EU4
{
	class SuperRegions: commonItems::parser
	{
	public:
		SuperRegions() = default;
		explicit SuperRegions(std::istream& theStream);
		
		[[nodiscard]] const auto& getSuperRegions() const { return superRegionMap; }

	private:
		std::map<std::string, std::vector<std::string>> superRegionMap;
	};
}

#endif // SUPER_REGIONS_H
