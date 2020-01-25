#ifndef EU4_LEADER_H
#define EU4_LEADER_H

#include "newParser.h"
#include "Date.h"

namespace EU4
{
	class Leader: commonItems::parser
	{
	public:
		Leader() = default;
		explicit Leader(std::istream& theStream);

		[[nodiscard]] const std::string& getName() const { return name; }
		[[nodiscard]] int getFire() const { return fire; }
		[[nodiscard]] int getShock() const { return shock; }
		[[nodiscard]] int getManeuver() const { return maneuver; }
		[[nodiscard]] int getSiege() const { return siege; }
		[[nodiscard]] int getID() const { return leaderID; }
		[[nodiscard]] date getActivationDate() const { return activationDate; }
		[[nodiscard]] bool isLand() const;

	private:
		std::string name;
		std::string leaderType;
		bool female = false;
		date activationDate;
		int fire = 0;
		int shock = 0;
		int maneuver = 0;
		int siege = 0;
		std::string country;
		int leaderID = 0;
	};
}

#endif // EU4_LEADER_H