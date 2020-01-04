#ifndef V2_LEADER_TRAIT_H_
#define V2_LEADER_TRAIT_H_

#include "newParser.h"


namespace mappers
{
	class V2LeaderTrait : commonItems::parser
	{
	public:
		V2LeaderTrait(std::istream& theStream);
		int getFire() const { return fire; }
		int getShock() const { return shock; }
		int getManuever() const { return manuever; }
		int getSiege() const { return siege; }
		int getOther() const { return other; }
		bool matches(int fire, int shock, int manuever, int siege) const;

	private:
		int fire = 0;
		int shock = 0;
		int manuever = 0;
		int siege = 0;
		int other = 0;
	};
}

#endif // V2_LEADER_TRAIT_H_