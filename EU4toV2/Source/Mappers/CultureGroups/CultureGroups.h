#ifndef CULTURE_GROUPS_H
#define CULTURE_GROUPS_H

#include "newParser.h"
#include "CultureGroup.h"
#include <map>
#include <optional>

namespace mappers
{
	class CultureGroups: commonItems::parser
	{
	public:
		CultureGroups();
		explicit CultureGroups(std::istream& theStream);
		
		[[nodiscard]] std::optional<CultureGroup> getCulturalGroup(const std::string& culture) const;
		[[nodiscard]] std::vector<Culture> getCulturesInGroup(const std::string& group) const;
		[[nodiscard]] const auto& getCultureToGroupMap() const { return cultureToGroupMap; }
		CultureGroup& retrieveCultureGroup(const std::string& culture);

	private:
		void registerKeys();
		
		std::map<std::string, std::vector<Culture>> groupToCulturesMap;
		std::map<std::string, CultureGroup> cultureToGroupMap;
	};
}

#endif // CULTURE_GROUPS_H
