#ifndef EU4_COLONIAL_REGIONS_H
#define EU4_COLONIAL_REGIONS_H

#include "newParser.h"
#include <map>

namespace EU4
{
	class ColonialRegions: commonItems::parser
	{
	public:
		ColonialRegions();
		[[nodiscard]] bool provinceIsInRegion(int province, const std::string& region) const;

	private:
		std::map<int, std::string> provinceToColonialRegions;
	};
}

#endif // EU4_COLONIAL_REGIONS_H