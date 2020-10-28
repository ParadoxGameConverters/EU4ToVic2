#ifndef COUNTRY_FLAGS_H
#define COUNTRY_FLAGS_H

#include "Parser.h"

namespace mappers
{
	class CountryFlags: commonItems::parser
	{
	public:
		CountryFlags();
		explicit CountryFlags(std::istream& theStream);

		void registerKeys();

		[[nodiscard]] const auto& getFlags() const { return flags; }
	
	private:
		std::multimap<std::string, std::string> flags;
	};
}


#endif // COUNTRY_FLAGS_H