#ifndef TECH_SCHOOLS
#define TECH_SCHOOLS

#include "../BlockedTechSchools/BlockedTechSchools.h"
#include "Parser.h"
#include "TechSchool.h"
#include <map>

namespace mappers
{
class TechSchools: commonItems::parser
{
  public:
	explicit TechSchools(std::istream& theStream);

	[[nodiscard]] const auto& getTechSchools() const { return techSchools; }

  private:
	std::map<std::string, TechSchool> techSchools;
	BlockedTechSchools blockedTechSchools;
};
} // namespace mappers

#endif // TECH_SCHOOLS