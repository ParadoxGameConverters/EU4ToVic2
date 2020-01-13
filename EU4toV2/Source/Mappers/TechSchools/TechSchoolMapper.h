#ifndef TECH_SCHOOL_MAPPER
#define TECH_SCHOOL_MAPPER

#include "TechSchools.h"
#include "../BlockedTechSchools/BlockedTechSchools.h"
#include "newParser.h"

namespace mappers
{
	class TechSchoolMapper: commonItems::parser
	{
	public:
		TechSchoolMapper();
		TechSchoolMapper(std::istream& theStream);
		std::string findBestTechSchool(
			double armyInvestment, 
			double commerceInvestment, 
			double cultureInvestment, 
			double industryInvestment, 
			double navyInvestment) const;

	private:
		void registerKeys();
		std::map<std::string, TechSchool> techSchools;
	};
}

#endif // TECH_SCHOOLS