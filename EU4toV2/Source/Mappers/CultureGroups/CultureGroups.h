#ifndef CULTURE_GROUPS_H
#define CULTURE_GROUPS_H

#include "newParser.h"
#include "CultureGroup.h"
#include <map>
#include <optional>

namespace EU4 {
	class World;
}

namespace mappers
{
	class CultureMapper;

	class CultureGroups: commonItems::parser
	{
	public:
		CultureGroups() = default;
		explicit CultureGroups(std::istream& theStream);

		void initForEU4();
		void initForV2();
		void importNeoCultures(const EU4::World& sourceWorld, const CultureMapper& cultureMapper);
		
		[[nodiscard]] std::optional<CultureGroup> getGroupForCulture(const std::string& cultureName) const;
		[[nodiscard]] std::map<std::string, Culture> getCulturesInGroup(const std::string& groupName) const;
		[[nodiscard]] const auto& getCultureGroupsMap() const { return cultureGroupsMap; }
		CultureGroup* retrieveCultureGroup(const std::string& cultureName);

		friend std::ostream& operator<<(std::ostream& output, const CultureGroups& cultureGroupsMapper);

	private:
		void registerKeys();
		
		std::map<std::string, CultureGroup> cultureGroupsMap;
	};
}

#endif // CULTURE_GROUPS_H
