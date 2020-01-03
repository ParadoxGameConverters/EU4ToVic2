#ifndef V2LEADER_H_
#define V2LEADER_H_

#include "Date.h"
#include "V2LeaderTraitMapper.h"
#include <ostream>
#include <string>
#include "../../EU4World/Leader/EU4Leader.h"


class V2Leader
{
	public:
		V2Leader() = default;
		V2Leader(const EU4::Leader& oldLeader, const mappers::V2LeaderTraitMapper& traitTypes);
		friend std::ostream& operator<<(std::ostream& output, const V2Leader& leader);
		int getEU4ID() const { return eu4ID; }

	private:
		std::string name;
		date activationDate;
		bool isLand = true;
		std::string personality;
		std::string background;
		int eu4ID = 0;
};

#endif // V2LEADER_H_