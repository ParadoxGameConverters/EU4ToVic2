#ifndef TECH_SCHOOLS
#define TECH_SCHOOLS

#include "V2TechSchools.h"
#include "BlockedTechSchools.h"
#include "Vic2TechSchool.h"
#include "newParser.h"
#include <memory>
#include <string>
#include <vector>



namespace Vic2
{

class TechSchools: commonItems::parser
{
	public:
		TechSchools(std::istream& theStream, std::unique_ptr<blockedTechSchools>& suppliedBlockedTechSchools);

		TechSchools() = default;
		TechSchools(const TechSchools&) = default;
		TechSchools(TechSchools&&) = default;
		TechSchools& operator=(const TechSchools&) = default;
		TechSchools& operator=(TechSchools&&) = default;
		~TechSchools() = default;

		std::string findBestTechSchool(double armyInvestment, double commerceInvestment, double cultureInvestment, double industryInvestment, double navyInvestment) const;

	private:
		std::vector<Vic2TechSchool> techSchools;
};


class TechSchoolsFile: commonItems::parser
{
	public:
		TechSchoolsFile(std::unique_ptr<blockedTechSchools> suppliedBlockedTechSchools);
		~TechSchoolsFile() = default;

		std::unique_ptr<TechSchools> takeTechSchools() { return std::move(theTechSchools); }

	private:
		TechSchoolsFile() = delete;
		TechSchoolsFile(const TechSchoolsFile&) = delete;
		TechSchoolsFile(TechSchoolsFile&&) = delete;
		TechSchoolsFile& operator=(const TechSchoolsFile&) = delete;
		TechSchoolsFile& operator=(TechSchoolsFile&&) = delete;

		std::unique_ptr<TechSchools> theTechSchools;
};

}



#endif // V2_TECH_SCHOOLS