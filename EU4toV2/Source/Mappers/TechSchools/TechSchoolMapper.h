#ifndef TECH_SCHOOL_MAPPER
#define TECH_SCHOOL_MAPPER

#include "Parser.h"
#include "TechSchools.h"

namespace mappers
{
class TechSchoolMapper: commonItems::parser
{
  public:
	TechSchoolMapper();
	explicit TechSchoolMapper(std::istream& theStream);

	[[nodiscard]] std::string findBestTechSchool(double armyInvestment,
		 double commerceInvestment,
		 double cultureInvestment,
		 double industryInvestment,
		 double navyInvestment) const;

  private:
	void registerKeys();

	std::map<std::string, TechSchool> techSchools;
};
} // namespace mappers

#endif // TECH_SCHOOLS