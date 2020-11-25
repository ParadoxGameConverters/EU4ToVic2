#ifndef CULTURAL_UNION_MAPPER_H
#define CULTURAL_UNION_MAPPER_H
#include "Parser.h"

namespace mappers
{
class CulturalUnionMapper: commonItems::parser
{
  public:
	CulturalUnionMapper(const std::string& filename);
	explicit CulturalUnionMapper(std::istream& theStream);

	[[nodiscard]] std::optional<std::vector<std::string>> getCoresForCulture(const std::string& culture) const;

  private:
	void registerKeys();
	std::map<std::string, std::vector<std::string>> unionMap;
};
} // namespace mappers

#endif // CULTURAL_UNION_MAPPER_H