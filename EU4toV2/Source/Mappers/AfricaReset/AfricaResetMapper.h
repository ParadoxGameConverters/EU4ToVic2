#ifndef AFRICA_RESET_MAPPER_H
#define AFRICA_RESET_MAPPER_H

#include "newParser.h"
#include <set>

namespace mappers
{
	class AfricaResetMapper: commonItems::parser
	{
	public:
		AfricaResetMapper();

		[[nodiscard]] auto isRegionResettable(const std::string& region) const { return resettableRegions.count(region) > 0; }
		[[nodiscard]] auto isTechResetable(int techLevel) const { return techLevel < reset_below_westernization; }

	private:
		std::set<std::string> resettableRegions;
		int reset_below_westernization = 5;
	};
}

#endif // AFRICA_RESET_MAPPER_H
