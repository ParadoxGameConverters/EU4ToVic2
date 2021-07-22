#ifndef PROVINCE_RENAME_H
#define PROVINCE_RENAME_H
#include "ConvenientParser.h"

namespace mappers
{
class ProvinceRename: commonItems::convenientParser
{
  public:
	explicit ProvinceRename(std::istream& theStream);

	[[nodiscard]] auto getProvinceID() const { return provinceID; }
	[[nodiscard]] const auto& getCulture() const { return culture; }
	[[nodiscard]] const auto& getName() const { return name; }

  private:
	void registerKeys();

	int provinceID = 0;
	std::string culture;
	std::string name;
};
} // namespace mappers

#endif // PROVINCE_RENAME_H