#ifndef AFRICA_RESET_MAPPER_H
#define AFRICA_RESET_MAPPER_H
#include "Parser.h"
#include <set>

namespace mappers
{
class AfricaResetMapper: commonItems::parser
{
  public:
	AfricaResetMapper();
	explicit AfricaResetMapper(std::istream& theStream);

	[[nodiscard]] auto isRegionResettable(const std::string& region) const { return resettableRegions.count(region) > 0; }
	[[nodiscard]] auto isTechResettable(int techLevel) const { return techLevel < reset_below_westernization; }

  private:
	void registerKeys();

	std::set<std::string> resettableRegions;
	int reset_below_westernization = 5;
};
} // namespace mappers

#endif // AFRICA_RESET_MAPPER_H
