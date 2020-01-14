#include "TechSchools.h"
#include "Log.h"

mappers::TechSchools::TechSchools(std::istream& theStream)
{
	registerKeyword(std::regex("[a-zA-Z0-9\\_]+"), [this](const std::string& name, std::istream& theStream)
		{
			TechSchool newTechSchool(theStream);
			if (!blockedTechSchools.isTechSchoolBlocked(name)) techSchools.insert(std::make_pair(name, newTechSchool));
		});

	parseStream(theStream);
	clearRegisteredKeywords();
}
