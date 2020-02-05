#ifndef SUPER_GROUP_MAPPER_H
#define SUPER_GROUP_MAPPER_H

#include "newParser.h"
#include <map>
#include <set>

namespace mappers
{
	class SuperGroupMapper : commonItems::parser
	{
	public:
		SuperGroupMapper();
		
		[[nodiscard]] std::optional<std::string> getGroupForSuperRegion(const std::string& superRegionName) const;

	private:
		std::map<std::string, std::set<std::string>> superGroups; // name (old_world) - superregions in it
	};
}

#endif // SUPER_GROUP_MAPPER_H