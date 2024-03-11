#ifndef PROVINCE_RENAMINGS_H
#define PROVINCE_RENAMINGS_H
#include "Parser.h"
#include <map>
#include <optional>

namespace mappers
{
class ProvinceRenamings: commonItems::parser
{
  public:
	ProvinceRenamings();
	explicit ProvinceRenamings(std::istream& theStream);

	[[nodiscard]] std::optional<std::string> getRename(int provinceID, const std::string& culture) const;

  private:
	void registerKeys();

	std::map<int, std::map<std::string, std::string>> renames; // province, <culture, name>
};
} // namespace mappers

#endif // PROVINCE_RENAMINGS_H