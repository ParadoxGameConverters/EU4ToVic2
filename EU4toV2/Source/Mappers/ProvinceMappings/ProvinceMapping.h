#ifndef PROVINCE_MAPPING_H
#define PROVINCE_MAPPING_H

#include "newParser.h"
#include <set>

namespace mappers
{
	class ProvinceMapping: commonItems::parser
	{
	public:
		explicit ProvinceMapping(std::istream& theStream);

		[[nodiscard]] const auto& getEU4Provinces() const { return eu4Provinces; }
		[[nodiscard]] const auto& getVic2Provinces() const { return vic2Provinces; }
		[[nodiscard]] const auto& getResettableRegions() const { return resettableRegions; }

	private:
		std::vector<int> eu4Provinces;
		std::vector<int> vic2Provinces;
		std::set<std::string> resettableRegions;
	};
}

#endif // PROVINCE_MAPPING_H