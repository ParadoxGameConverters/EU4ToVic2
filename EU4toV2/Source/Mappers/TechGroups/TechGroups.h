#ifndef TECH_GROUPS_H
#define TECH_GROUPS_H

#include "newParser.h"

namespace mappers
{
	class TechGroups: commonItems::parser
	{
	public:
		explicit TechGroups(std::istream& theStream);
		
		[[nodiscard]] auto getWesternization() const { return westernization; }
		[[nodiscard]] auto getLiteracyBoost() const { return literacyBoost; }

	private:
		// Values range 0-10. 5 is default.
		int westernization = 5;
		int literacyBoost = 5;
	};
}

#endif // TECH_GROUPS_H