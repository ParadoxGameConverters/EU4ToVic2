#ifndef BLOCKED_TECH_SCHOOLS_H
#define BLOCKED_TECH_SCHOOLS_H

#include "newParser.h"
#include <string>
#include <vector>

namespace mappers
{
	class BlockedTechSchools : commonItems::parser
	{
	public:
		BlockedTechSchools();
		BlockedTechSchools(std::istream& theStream);

		bool isTechSchoolBlocked(const std::string& techSchool) const;

	private:
		void registerKeys();
		std::vector<std::string> theBlockedTechSchools;
	};
}

#endif // BLOCKED_TECH_SCHOOLS_H