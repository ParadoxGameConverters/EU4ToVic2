#ifndef PROVINCE_MAPPING_H
#define PROVINCE_MAPPING_H

#include "newParser.h"
#include <set>

namespace mappers
{
	class ProvinceMapping: commonItems::parser
	{
	public:
		ProvinceMapping(std::istream& theStream);

		std::vector<int> getEU4Provinces() const { return EU4Provinces; }
		std::vector<int> getVic2Provinces() const { return Vic2Provinces; }
		std::set<std::string> getResettableRegions() const { return resettableRegions; }

	private:
		std::vector<int> EU4Provinces;
		std::vector<int> Vic2Provinces;
		std::set<std::string> resettableRegions;
	};
}

#endif // PROVINCE_MAPPING_H