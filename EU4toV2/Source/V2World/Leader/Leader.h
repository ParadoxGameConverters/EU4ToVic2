#ifndef LEADER_H
#define LEADER_H

#include "Date.h"
#include <string>
#include "../../Mappers/LeaderTraits/LeaderTraitMapper.h"
#include "../../EU4World/Leader/EU4Leader.h"

namespace V2
{
	class Leader
	{
	public:
		Leader() = default;
		Leader(const EU4::Leader& oldLeader, const mappers::LeaderTraitMapper& leaderTraitMapper);
		int getEU4ID() const { return eu4ID; }

		friend std::ostream& operator<<(std::ostream& output, const Leader& leader);

	private:
		std::string name;
		date activationDate;
		bool isLand = true;
		std::string personality;
		std::string background;
		int eu4ID = 0;
	};
}

#endif // LEADER_H