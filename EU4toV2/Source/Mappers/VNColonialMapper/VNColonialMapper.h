#ifndef VN_COLONIAL_MAPPER
#define VN_COLONIAL_MAPPER
#include "Parser.h"
#include "VNColonialMapping.h"
#include <set>

namespace mappers
{
class VNColonialMapper: commonItems::parser
{
  public:
	VNColonialMapper();
	explicit VNColonialMapper(std::istream& theStream);

	[[nodiscard]] const auto& getVNColonies() const { return VNColonies; }
	[[nodiscard]] bool isProvinceVNColonial(const int provinceID) const { return allColonialProvinces.contains(provinceID); }

  private:
	void registerKeys();

	std::vector<VNColonialMapping> VNColonies;
	std::set<int> allColonialProvinces;
};
} // namespace mappers

#endif // VN_COLONIAL_MAPPER