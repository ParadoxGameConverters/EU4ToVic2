#include "MinorityPopMapper.h"
#include "MinorityPop.h"
#include "ParserHelpers.h"

mappers::MinorityPopMapper::MinorityPopMapper()
{
	registerKeys();
	parseFile("configurables/minority_pops.txt");
	clearRegisteredKeywords();
}

mappers::MinorityPopMapper::MinorityPopMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::MinorityPopMapper::registerKeys()
{
	registerKeyword("minority", [this](const std::string& unused, std::istream& theStream)
		{
			MinorityPop minPop(theStream);
			minorityPopMap.push_back(make_pair(minPop.getCulture(), minPop.getReligion()));
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
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
