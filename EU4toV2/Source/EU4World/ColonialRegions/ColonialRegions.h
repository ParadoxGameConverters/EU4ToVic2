#ifndef EU4_COLONIAL_REGIONS_H
#define EU4_COLONIAL_REGIONS_H

#include "Parser.h"
#include <map>

namespace EU4
{
class ColonialRegions: commonItems::parser
{
  public:
	ColonialRegions();
	explicit ColonialRegions(std::istream& theStream);

	[[nodiscard]] bool provinceIsInColonialRegion(int province, const std::string& region) const;

  private:
	void registerKeys();

	std::map<int, std::string> provinceToColonialRegions;
};
} // namespace EU4

#endif // EU4_COLONIAL_REGIONS_H