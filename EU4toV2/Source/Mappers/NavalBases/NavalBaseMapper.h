#ifndef MAP_PROVINCE_H
#define MAP_PROVINCE_H

#include "Parser.h"
#include <set>

namespace mappers
{
	class NavalBaseMapper : commonItems::parser
	{
	public:
		NavalBaseMapper();
		
		[[nodiscard]] const auto& getNavalProvinces() const { return navalProvinces; }
		[[nodiscard]] auto isProvinceCoastal(int provID) const { return navalProvinces.count(provID) > 0; }

	private:
		std::set<int> navalProvinces;
	};
}

#endif // MAP_PROVINCE_H
