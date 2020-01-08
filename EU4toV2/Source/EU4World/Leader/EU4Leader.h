#ifndef EU4_LEADER_H_
#define EU4_LEADER_H_

#include "newParser.h"
#include "Date.h"

namespace EU4
{
	class Leader: commonItems::parser
	{
		public:
			Leader() = default;
			Leader(std::istream& theStream);

			std::string getName() const { return name; }
			int getFire() const { return fire; }
			int getShock() const { return shock; }
			int getManuever() const { return manuever; }
			int getSiege() const { return siege; }
			int getID() const { return leaderID; }
			date getActivationDate() const { return activationDate; }

			bool isLand() const;

		private:
			std::string name;
			std::string leaderType;
			bool female = false;
			date activationDate;
			int fire = 0;
			int shock = 0;
			int manuever = 0;
			int siege = 0;
			std::string country;
			int leaderID = 0;
	};
}

#endif // EU4_LEADER_H_