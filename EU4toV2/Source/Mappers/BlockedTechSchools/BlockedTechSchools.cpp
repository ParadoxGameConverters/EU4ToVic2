#include "BlockedTechSchools.h"
#include "ParserHelpers.h"
#include "OSCompatibilityLayer.h"

mappers::BlockedTechSchools::BlockedTechSchools()
{
	registerKeys();
	if (!commonItems::DoesFileExist("configurables/blocked_tech_schools.txt"))
	{
		Log(LogLevel::Warning) << "Could not find configurables/blocked_tech_schools.txt";
	}
	else
	{
		parseFile("configurables/blocked_tech_schools.txt");
	}

	clearRegisteredKeywords();
}

mappers::BlockedTechSchools::BlockedTechSchools(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::BlockedTechSchools::registerKeys()
{
	registerKeyword("blocked", [this](const std::string& techSchool, std::istream& theStream) 
		{
			const commonItems::singleString blockedStr(theStream);
			theBlockedTechSchools.push_back(blockedStr.getString());
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}

bool mappers::BlockedTechSchools::isTechSchoolBlocked(const std::string& techSchool) const
{
	return std::any_of(theBlockedTechSchools.begin(), theBlockedTechSchools.end(),
							 [techSchool](const std::string& blockedTechSchool){ return techSchool == blockedTechSchool; });
}

