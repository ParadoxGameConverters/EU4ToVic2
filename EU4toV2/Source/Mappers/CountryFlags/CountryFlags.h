#ifndef COUNTRY_FLAGS_H
#define COUNTRY_FLAGS_H

#include "Parser.h"

namespace mappers
{
	class CountryFlags: commonItems::parser
	{
	public:
		CountryFlags();

		void registerKeys();

		[[nodiscard]] const auto& getFlags() const { return flags; }
	
	private:
		std::map<std::string, std::vector<std::string>> flags;
	};
}


#endif // COUNTRY_FLAGS_H