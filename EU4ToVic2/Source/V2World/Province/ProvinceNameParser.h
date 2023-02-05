#ifndef PROVINCE_NAME_PARSER_H
#define PROVINCE_NAME_PARSER_H
#include <map>
#include <optional>
#include <string>

namespace V2
{
class ProvinceNameParser
{
  public:
	ProvinceNameParser();

	[[nodiscard]] std::optional<std::string> getProvinceName(int provID) const;

  private:
	std::map<int, std::string> provinceNames;

	void importProvinceLocalizations(const std::string& file);
};
} // namespace V2
#endif // PROVINCE_NAME_PARSER_H
