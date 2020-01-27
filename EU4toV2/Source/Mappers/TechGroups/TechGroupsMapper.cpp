#include "TechGroupsMapper.h"
#include "TechGroups.h"
#include "Log.h"

mappers::TechGroupsMapper::TechGroupsMapper()
{
	LOG(LogLevel::Info) << "Parsing technology groups.";
	registerRegex("[a-zA-Z0-9_]+", [this](const std::string& techGroup, std::istream& theStream)
		{
			const TechGroups techGroups(theStream);

			westernizations[techGroup] = techGroups.getWesternization();
			literacies[techGroup] = techGroups.getLiteracyBoost();
		});

	parseFile("configurables/tech_groups.txt");
	clearRegisteredKeywords();
}

int mappers::TechGroupsMapper::getWesternizationFromTechGroup(const std::string& techGroupName) const
{
	const auto& techGroup = westernizations.find(techGroupName);
	if (techGroup != westernizations.end()) return techGroup->second;
	LOG(LogLevel::Warning) << "Unknown tech group: " << techGroupName;
	return 0;
}

int mappers::TechGroupsMapper::getLiteracyFromTechGroup(const std::string& techGroupName) const
{
	const auto& techGroup = literacies.find(techGroupName);
	if (techGroup != literacies.end()) return techGroup->second;
	LOG(LogLevel::Warning) << "Unknown tech group: " << techGroupName;
	return 0;
}

