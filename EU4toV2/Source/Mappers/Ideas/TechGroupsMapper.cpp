#include "TechGroupsMapper.h"
#include "TechGroups.h"
#include "Log.h"


mappers::TechGroupsMapper::TechGroupsMapper()
{
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this](const std::string& techGroup, std::istream& theStream) {
		TechGroups techGroups(theStream);

		westernizations[techGroup] = techGroups.getWesternization();
		literacies[techGroup] = techGroups.getLiteracyBoost();
	});

	LOG(LogLevel::Info) << "getting tech groups";
	parseFile("techGroups.txt");
}


int mappers::TechGroupsMapper::getWesternizationFromTechGroup(const std::string& techGroupName) const
{
	auto techGroup = westernizations.find(techGroupName);
	return techGroup->second;
}


int mappers::TechGroupsMapper::getLiteracyFromTechGroup(const std::string& techGroupName) const
{
	auto techGroup = literacies.find(techGroupName);
	return techGroup->second;
}

