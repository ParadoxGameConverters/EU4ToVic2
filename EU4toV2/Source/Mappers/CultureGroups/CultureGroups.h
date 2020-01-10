#ifndef CULTURE_GROUPS_H
#define CULTURE_GROUPS_H

#include "newParser.h"
#include "CultureGroup.h"
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace mappers
{
	class CultureGroups : commonItems::parser
	{
		public:
			CultureGroups();
			std::optional<CultureGroup> getCulturalGroup(const std::string& culture);
			const std::map<std::string, CultureGroup>& getCultureToGroupMap() const { return cultureToGroupMap; }
			const std::vector<Culture>& getCulturesInGroup(const std::string& group);

		private:
			void registerKeys();
			std::map<std::string, std::vector<Culture>> groupToCulturesMap;
			std::map<std::string, CultureGroup> cultureToGroupMap;
	};
}

#endif // CULTURE_GROUPS_H
