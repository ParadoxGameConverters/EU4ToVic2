#ifndef EU4_LEADER_H
#define EU4_LEADER_H

#include "Parser.h"
#include "Date.h"

namespace EU4
{
	class Leader: commonItems::parser
	{
	public:
		Leader() = default;
		explicit Leader(std::istream& theStream);

		[[nodiscard]] const auto& getName() const { return name; }
		[[nodiscard]] auto getFire() const { return fire; }
		[[nodiscard]] auto getShock() const { return shock; }
		[[nodiscard]] auto getManeuver() const { return maneuver; }
		[[nodiscard]] auto getSiege() const { return siege; }
		[[nodiscard]] auto getID() const { return leaderID; }
		[[nodiscard]] const auto& getActivationDate() const { return activationDate; }
		
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