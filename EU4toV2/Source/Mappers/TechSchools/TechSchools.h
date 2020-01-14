#ifndef TECH_SCHOOLS
#define TECH_SCHOOLS

#include "TechSchool.h"
#include "../BlockedTechSchools/BlockedTechSchools.h"
#include "newParser.h"
#include <map>

namespace mappers
{
	class TechSchools: commonItems::parser
	{
	public:
		TechSchools(std::istream& theStream);
		std::map<std::string, TechSchool> getTechSchools() const { return techSchools; }

	private:
		std::map<std::string, TechSchool> techSchools;

		mappers::BlockedTechSchools blockedTechSchools;
	};
}

#endif // TECH_SCHOOLS