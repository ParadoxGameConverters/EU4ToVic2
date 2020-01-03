#include "V2LeaderTrait.h"
#include "ParserHelpers.h"
#include "Log.h"


mappers::V2LeaderTrait::V2LeaderTrait(std::istream& theStream)
{
	registerKeyword(std::regex("fire"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt fireStr(theStream);
			fire = fireStr.getInt();
		});
	registerKeyword(std::regex("shock"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt shockStr(theStream);
			shock = shockStr.getInt();
		});
	registerKeyword(std::regex("manuever"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt manueverStr(theStream);
			manuever = manueverStr.getInt();
		});
	registerKeyword(std::regex("siege"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt siegeStr(theStream);
			siege = siegeStr.getInt();
		});
	registerKeyword(std::regex("other"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt otherStr(theStream);
			other = otherStr.getInt();
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}

bool mappers::V2LeaderTrait::matches(int leaderFire, int leaderShock, int leaderManuever, int leaderSiege) const
{
	if ((leaderFire < fire) || (leaderShock < shock) || (leaderManuever < manuever) || (leaderSiege < siege))
	{
		return false;
	}

	// other consists of the sum of all non-required attributes
	if (other > 0)
	{
		int incOther = (fire != 0 ? 0 : leaderFire);
		incOther += (shock != 0 ? 0 : leaderShock);
		incOther += (manuever != 0 ? 0 : leaderManuever);
		incOther += (siege != 0 ? 0 : leaderSiege);

		if (incOther < other)
		{
			return false;
		}
	}
	return true;
}
