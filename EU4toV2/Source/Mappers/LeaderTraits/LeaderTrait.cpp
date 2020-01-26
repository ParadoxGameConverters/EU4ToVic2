#include "LeaderTrait.h"
#include "ParserHelpers.h"
#include "Log.h"

mappers::LeaderTrait::LeaderTrait(std::istream& theStream)
{
	registerKeyword("fire", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleInt fireStr(theStream);
			fire = fireStr.getInt();
		});
	registerKeyword("shock", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleInt shockStr(theStream);
			shock = shockStr.getInt();
		});
	registerKeyword("maneuver", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleInt manueverStr(theStream);
			maneuver = manueverStr.getInt();
		});
	registerKeyword("siege", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleInt siegeStr(theStream);
			siege = siegeStr.getInt();
		});
	registerKeyword("other", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleInt otherStr(theStream);
			other = otherStr.getInt();
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}

bool mappers::LeaderTrait::matches(const int& leaderFire, const int& leaderShock, const int& leaderManeuver, const int& leaderSiege) const
{
	if (leaderFire < fire || leaderShock < shock || leaderManeuver < maneuver || leaderSiege < siege)
	{
		return false;
	}

	// other consists of the sum of all non-required attributes
	if (other > 0)
	{
		auto incOther = fire != 0 ? 0 : leaderFire;
		incOther += shock != 0 ? 0 : leaderShock;
		incOther += maneuver != 0 ? 0 : leaderManeuver;
		incOther += siege != 0 ? 0 : leaderSiege;

		if (incOther < other) return false;
	}
	return true;
}
