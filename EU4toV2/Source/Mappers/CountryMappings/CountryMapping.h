#ifndef COUNTRY_MAPPING_H
#define COUNTRY_MAPPING_H

#include "newParser.h"

namespace mappers
{
	class CountryMapping: commonItems::parser
	{
	public:
		explicit CountryMapping(std::istream& theStream);

		[[nodiscard]] const auto& getEU4Tag() const { return eu4Tag; }
		[[nodiscard]] const auto& getVic2Tag() const { return vic2Tag; }

	private:
		std::string eu4Tag;
		std::string vic2Tag;
	};
}

#endif // COUNTRY_MAPPING_H