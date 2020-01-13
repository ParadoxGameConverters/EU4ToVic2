#include "TechSchools.h"
#include "../../Configuration.h"
#include "Log.h"
#include "ParserHelpers.h"
#include <algorithm>
#include <memory>

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
