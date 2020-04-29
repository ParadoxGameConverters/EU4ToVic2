#ifndef COUNTRY_MAPPING_H
#define COUNTRY_MAPPING_H

#include "Parser.h"
#include <set>

namespace mappers
{
	class CountryMapping: commonItems::parser
	{
	public:
		CountryMapping() = default;
		explicit CountryMapping(std::istream& theStream);

		[[nodiscard]] const auto& getEU4Tag() const { return eu4Tag; }
		[[nodiscard]] const auto& getVic2Tag() const { return vic2Tag; }
		[[nodiscard]] const auto& getReforms() const { return reforms; }
		[[nodiscard]] const auto& getFlags() const { return flags; }

	private:
		std::string eu4Tag;
		std::string vic2Tag;
		std::set<std::string> reforms;
		std::set<std::string> flags;
	};
}

#endif // COUNTRY_MAPPING_H