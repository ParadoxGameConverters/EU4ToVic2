#ifndef LEADER_TRAIT_H
#define LEADER_TRAIT_H

#include "newParser.h"

namespace mappers
{
	class LeaderTrait : commonItems::parser
	{
	public:
		explicit LeaderTrait(std::istream& theStream);
		
		[[nodiscard]] auto getFire() const { return fire; }
		[[nodiscard]] auto getShock() const { return shock; }
		[[nodiscard]] auto getManeuver() const { return maneuver; }
		[[nodiscard]] auto getSiege() const { return siege; }
		[[nodiscard]] auto getOther() const { return other; }
		
		[[nodiscard]] bool matches(const int& leaderFire, const int& leaderShock, const int& leaderManeuver, const int& leaderSiege) const;

	private:
		int fire = 0;
		int shock = 0;
		int maneuver = 0;
		int siege = 0;
		int other = 0;
	};
}

#endif // LEADER_TRAIT_H