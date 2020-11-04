#ifndef DATE_ITEMS_H
#define DATE_ITEMS_H
#include "Parser.h"
#include <vector>

namespace EU4
{
class DateItems: commonItems::parser
{
  public:
	explicit DateItems(std::istream& theStream);

	[[nodiscard]] const auto& getDateChanges() const { return dateChanges; }

  private:
	void registerKeys();

	std::vector<std::pair<std::string, std::string>> dateChanges; // changeType, changeValue
};
} // namespace EU4

#endif // DATE_ITEMS_H