#ifndef LEADER_H
#define LEADER_H

#include "../../EU4World/Leader/EU4Leader.h"
#include "../../Mappers/LeaderTraits/LeaderTraitMapper.h"
#include "Date.h"
#include <string>

namespace V2
{
class Leader
{
  public:
	Leader() = default;
	Leader(const EU4::Leader& oldLeader, const mappers::LeaderTraitMapper& leaderTraitMapper);

	friend std::ostream& operator<<(std::ostream& output, const Leader& leader);

  private:
	std::string name;
	date activationDate;
	bool isLand = true;
	std::string personality;
	std::string background;
};
} // namespace V2

#endif // LEADER_H