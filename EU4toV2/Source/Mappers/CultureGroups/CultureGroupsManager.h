#ifndef CULTURE_GROUPS_MANAGER_H
#define CULTURE_GROUPS_MANAGER_H
#include "../CultureGroupsMapper/CultureGroupsMapper.h"
#include "Parser.h"
#include <set>

namespace EU4
{
class Regions;
}

namespace mappers
{
class CultureGroup;
class CultureMapper;
class CultureGroupsManager: commonItems::parser
{
  public:
	CultureGroupsManager() = default;
	explicit CultureGroupsManager(std::istream& theStream);

	void initForEU4();
	void initForV2();
	void importNeoCultures(const EU4::Regions& regions,
		 const std::shared_ptr<CultureGroupsManager>& eu4CultureGroupsMapper,
		 const CultureMapper& cultureMapper) const;
	void importDynamicCultures(const std::shared_ptr<CultureGroupsManager>& eu4CultureGroupsMapper);

	[[nodiscard]] std::shared_ptr<CultureGroup> getGroupForCulture(const std::string& cultureName) const;
	[[nodiscard]] const auto& getCultureGroupsMap() const { return cultureGroupsMap; }
	[[nodiscard]] const auto& getDynamicCultureNames() const { return importedDynamicCultures; }

	friend std::ostream& operator<<(std::ostream& output, const CultureGroupsManager& cultureGroupsManager);

  private:
	void registerKeys();

	std::map<std::string, std::shared_ptr<CultureGroup>> cultureGroupsMap;
	std::set<std::string> importedDynamicCultures; // cache to ease localization dump.

	CultureGroupsMapper cultureGroupsMapper;
};
} // namespace mappers

#endif // CULTURE_GROUPS_MANAGER_H
