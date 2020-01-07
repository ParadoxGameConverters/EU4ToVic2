#ifndef EU4_REGION_H
#define EU4_REGION_H

#include "newParser.h"
#include "Areas.h"
#include <set>

namespace EU4
{
	class Region: commonItems::parser
	{
		public:
			Region(std::istream& theStream);
			Region(std::set<int>& _provinces);
			bool containsProvince(unsigned int province) const;
			void addProvinces(const EU4::Areas& areas);

		private:
			std::set<std::string> areaNames;
			std::set<int> provinces;
	};
}

#endif // EU4_REGION_H