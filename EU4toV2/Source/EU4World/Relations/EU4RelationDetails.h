#ifndef EU4RELATION_DETAILS_H
#define EU4RELATION_DETAILS_H

#include "Date.h"
#include "newParser.h"

namespace EU4
{
	class EU4RelationDetails :  commonItems::parser
	{
	public:
		explicit EU4RelationDetails(std::istream& theStream);
		
		[[nodiscard]] auto getRelations() const { return value; }
		[[nodiscard]] auto hasMilitaryAccess() const { return military_access; }
		[[nodiscard]] auto getDiplomatLastSent() const { return last_send_diplomat; }
		[[nodiscard]] auto getLastWar() const { return last_war; }
		[[nodiscard]] auto getAttitude() const { return attitude; }

	private:
		int value = 0; // the relationship value
		bool military_access = false; // if military access has been granted
		date last_send_diplomat; // the last diplomat sent between the countries
		date last_war; // the last war between the countries
		std::string attitude = "attitude_neutral"; // the attitude, if there is one
	};
}

#endif // EU4RELATION_DETAILS_H