#ifndef EU4WAR_DETAILS_H
#define EU4WAR_DETAILS_H

#include "Date.h"
#include "Parser.h"

namespace EU4
{
	class WarDetails :  commonItems::parser
	{
	public:
		WarDetails() = default;
		void addDetails(std::istream& theStream);

		// This is a storage container.
		std::string warGoalType;
		int targetProvinceID = 0;
		std::string targetTag;
		std::string EU4casusBelli; // unused at the moment, we're using warGoalType 
		date startDate = date("1.1.1");
		std::string warGoalClass;
	};
}

#endif // EU4WAR_DETAILS_H