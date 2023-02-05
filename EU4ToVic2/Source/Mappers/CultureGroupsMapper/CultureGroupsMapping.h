#ifndef CULTURE_GROUPS_MAPPING
#define CULTURE_GROUPS_MAPPING
#include "Parser.h"
#include <set>

namespace mappers
{
class CultureGroupsMapping: commonItems::parser
{
  public:
	explicit CultureGroupsMapping(std::istream& theStream);

	[[nodiscard]] const auto& getV2() const { return v2; }
	[[nodiscard]] const auto& getEU4s() const { return eu4s; }

  private:
	void registerKeys(bool isHPMEnabled);

	std::string v2;
	std::optional<std::string> hpm;
	std::set<std::string> eu4s;
};
} // namespace mappers

#endif // CULTURE_GROUPS_MAPPING