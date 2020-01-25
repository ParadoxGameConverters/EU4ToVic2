#ifndef EU4_COUNTRY_FLAGS_H
#define EU4_COUNTRY_FLAGS_H

#include "newParser.h"
#include <set>

namespace EU4
{
	class EU4CountryFlags: commonItems::parser
	{
	public:
		EU4CountryFlags() = default;
		explicit EU4CountryFlags(std::istream& theStream);
		[[nodiscard]] const auto& getFlags() const { return flags; }

	private:
		std::set<std::string> flags;
	};
}

#endif // EU4_COUNTRY_FLAGS_H