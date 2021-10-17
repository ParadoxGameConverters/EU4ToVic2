#ifndef VN_COLONIAL_MAPPING
#define VN_COLONIAL_MAPPING
#include "Parser.h"

namespace mappers
{
class VNColonialMapping: commonItems::parser
{
  public:
	explicit VNColonialMapping(std::istream& theStream);

	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] auto getKeyProvince() const { return keyProvince; }
	[[nodiscard]] const auto& getProvinces() const { return provinces; }

  private:
	void registerKeys();

	std::string name;
	int keyProvince = 0;
	std::vector<int> provinces;
};
} // namespace mappers

#endif // VN_COLONIAL_MAPPING