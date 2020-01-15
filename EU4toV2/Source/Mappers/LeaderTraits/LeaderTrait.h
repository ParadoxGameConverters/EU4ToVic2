#ifndef LEADER_TRAIT_H
#define LEADER_TRAIT_H

#include "newParser.h"

namespace mappers
{
	class LeaderTrait : commonItems::parser
	{
	public:
		LeaderTrait(std::istream& theStream);
		int getFire() const { return fire; }
		int getShock() const { return shock; }
		int getManeuver() const { return maneuver; }
		int getSiege() const { return siege; }
		int getOther() const { return other; }
		bool matches(const int& leaderFire, const int& leaderShock, const int& leaderManeuver, const int& leaderSiege) const;

	private:
		int fire = 0;
		int shock = 0;
		int maneuver = 0;
		int siege = 0;
		int other = 0;
	};
}

#endif // LEADER_TRAIT_H