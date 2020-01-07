#include "MinorityPopMapper.h"
#include "MinorityPop.h"
#include "../V2World/V2Pop.h"
#include <fstream>
#include "ParserHelpers.h"

mappers::MinorityPopMapper::MinorityPopMapper()
{
	registerKeyword(std::regex("minority"), [this](const std::string& unused, std::istream& theStream)
	{
		MinorityPop minPop(theStream);
		minorityPopMap.push_back(make_pair(minPop.getCulture(), minPop.getReligion()));
	});
	registerKeyword(std::regex("[a-z0-9\\_]+"), commonItems::ignoreItem);

	parseFile("minorityPops.txt");
}

bool mappers::MinorityPopMapper::matchMinorityPop(V2Pop& pop) const
{
	for (auto minorityItr : minorityPopMap)
	{
		if ((pop.getCulture() == minorityItr.first) && (pop.getReligion() == minorityItr.second))
		{
			return true;
		}
		else if ((minorityItr.first == "") && (pop.getReligion() == minorityItr.second))
		{
			pop.setCulture("");
			return true;
		}
		else if ((pop.getCulture() == minorityItr.first) && (minorityItr.second == ""))
		{
			pop.setReligion("");
			return true;
		}
	}

	return false;
}
