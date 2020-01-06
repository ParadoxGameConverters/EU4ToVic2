#ifndef EU4_COUNTRY_FLAGS_H_
#define EU4_COUNTRY_FLAGS_H_

#include "newParser.h"
#include <set>

namespace EU4
{
	class EU4CountryFlags: commonItems::parser
	{
		public:
			EU4CountryFlags() = default;
			EU4CountryFlags(std::istream& theStream);
			std::set<std::string> getFlags() const { return flags; }

		private:
			std::set<std::string> flags;
	};
}

#endif // EU4_COUNTRY_FLAGS_H_