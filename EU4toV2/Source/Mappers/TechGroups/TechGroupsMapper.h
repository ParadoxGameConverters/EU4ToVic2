#ifndef TECH_GROUPS_MAPPER_H
#define TECH_GROUPS_MAPPER_H

#include "newParser.h"
#include <map>

namespace mappers
{
	class TechGroupsMapper: commonItems::parser
	{
	public:
		TechGroupsMapper();

		int getWesternizationFromTechGroup(const std::string& techGroup) const;
		int getLiteracyFromTechGroup(const std::string& ideaName) const;

	private:
		std::map<std::string, int> westernizations;
		std::map<std::string, int> literacies;
	};
}

#endif // TECH_GROUPS_MAPPER_H
