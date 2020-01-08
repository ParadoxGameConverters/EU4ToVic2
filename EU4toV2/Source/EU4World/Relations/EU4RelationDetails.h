#ifndef EU4RELATION_DETAILS_H
#define EU4RELATION_DETAILS_H

#include "Date.h"
#include "newParser.h"

namespace EU4
{
	class EU4RelationDetails :  commonItems::parser
	{
	public:
		EU4RelationDetails(std::istream& theStream);
		int getRelations() const { return value; }
		bool hasMilitaryAccess() const { return military_access; }
		date getDiplomatLastSent() const { return last_send_diplomat; }
		date getLastWar() const { return last_war; }
		std::string getAttitude() const { return attitude; }

	private:
		int value = 0; // the relationship value
		bool military_access = false; // if military access has been granted
		date last_send_diplomat; // the last diplomat sent between the countries
		date last_war; // the last war between the countries
		std::string attitude = "attitude_neutral"; // the attitude, if there is one
	};
}

#endif // EU4RELATION_DETAILS_H