#ifndef MAP_PROVINCE_H
#define MAP_PROVINCE_H

#include "newParser.h"
#include <set>

namespace mappers
{
	class NavalBaseMapper : commonItems::parser
	{
	public:
		NavalBaseMapper();
		const std::set<int>& getNavalProvinces() const { return navalProvinces; }
		bool isProvinceCoastal(int provID) const { return navalProvinces.count(provID) > 0; }

	private:
		std::set<int> navalProvinces;
	};
}

#endif // MAP_PROVINCE_H
