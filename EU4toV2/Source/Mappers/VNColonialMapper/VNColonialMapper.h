#ifndef VN_COLONIAL_MAPPER
#define VN_COLONIAL_MAPPER
#include "Parser.h"
#include "VNColonialMapping.h"

namespace mappers
{
class VNColonialMapper: commonItems::parser
{
  public:
	VNColonialMapper();
	explicit VNColonialMapper(std::istream& theStream);

	[[nodiscard]] const auto& getVNColonies() const { return VNColonies; }

  private:
	void registerKeys();

	std::vector<VNColonialMapping> VNColonies;
};
} // namespace mappers

#endif // VN_COLONIAL_MAPPER