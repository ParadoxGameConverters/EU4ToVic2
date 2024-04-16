#ifndef TECH_GROUPS_H
#define TECH_GROUPS_H

#include "Parser.h"
#include <set>

namespace mappers
{
class TechGroups: commonItems::parser
{
  public:
	explicit TechGroups(std::istream& theStream);

	[[nodiscard]] auto getWesternization() const { return westernization; }
	[[nodiscard]] auto getLiteracyBoost() const { return literacyBoost; }
	[[nodiscard]] const auto& getCultures() const { return cultures; }
	[[nodiscard]] const auto& getIndustry() const { return industry; }

  private:
	// Values range 0-10. 5 is default.
	int westernization = 5;
	int literacyBoost = 5;
	int industry = 5;
	std::set<std::string> cultures;
	std::set<std::string> hpmCultures;
	int hpmWesternization = 0;
};
} // namespace mappers

#endif // TECH_GROUPS_H