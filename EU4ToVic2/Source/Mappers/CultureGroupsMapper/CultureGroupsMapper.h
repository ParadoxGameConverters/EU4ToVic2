#ifndef CULTURE_GROUPS_MAPPER_H
#define CULTURE_GROUPS_MAPPER_H
#include "Parser.h"
#include <map>

namespace mappers
{
class CultureGroupsMapper: commonItems::parser
{
  public:
	CultureGroupsMapper();
	explicit CultureGroupsMapper(std::istream& theStream);

	[[nodiscard]] std::optional<std::string> getV2CultureGroup(const std::string& eu4CultureGroup) const;

  private:
	void registerKeys();

	std::map<std::string, std::string> eu4ToV2CultureGroups;
};
} // namespace mappers

#endif // CULTURE_GROUPS_MAPPER_H