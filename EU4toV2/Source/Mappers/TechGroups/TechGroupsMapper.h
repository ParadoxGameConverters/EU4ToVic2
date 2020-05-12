#ifndef TECH_GROUPS_MAPPER_H
#define TECH_GROUPS_MAPPER_H

#include "Parser.h"
#include <map>

namespace mappers
{
	class TechGroupsMapper: commonItems::parser
	{
	public:
		TechGroupsMapper();

		[[nodiscard]] int getWesternizationFromTechGroup(const std::string& techGroupName) const;
		[[nodiscard]] int getLiteracyFromTechGroup(const std::string& techGroupName) const;

	private:
		std::map<std::string, int> westernizations;
		std::map<std::string, int> literacies;
	};
}

#endif // TECH_GROUPS_MAPPER_H
