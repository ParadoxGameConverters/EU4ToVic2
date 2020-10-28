#ifndef COUNTRY_FLAGS_MAPPING_H
#define COUNTRY_FLAGS_MAPPING_H

#include "Parser.h"

namespace mappers
{
class CountryFlagsMapping: commonItems::parser
{
  public:
	CountryFlagsMapping(std::istream& theStream);

	void registerKeys();

	[[nodiscard]] const auto& getEU4Flag() const { return eu4Flag; }
	[[nodiscard]] const auto& getV2Flags() const { return v2Flags; }

  private:
	std::string eu4Flag;
	std::vector<std::string> v2Flags;
};
} // namespace mappers

#endif // COUNTRY_FLAGS_MAPPING_H