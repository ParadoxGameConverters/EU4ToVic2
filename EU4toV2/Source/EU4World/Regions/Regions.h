#ifndef EU4_REGIONS_H
#define EU4_REGIONS_H

#include "newParser.h"
#include "Region.h"
#include "Areas.h"
#include <map>

namespace EU4
{
	class Regions: commonItems::parser
	{
		public:
			Regions() = default;
			Regions(const EU4::Areas& areas, std::istream& regionsFile);
			Regions(const EU4::Areas& areas);
			virtual bool provinceInRegion(int province, const std::string& regionName) const;

		private:
			std::map<std::string, EU4::Region> regions;
	};
}

#endif //EU4_REGIONS_H
