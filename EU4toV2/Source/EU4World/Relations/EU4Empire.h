#ifndef EU4EMPIRE_H
#define EU4EMPIRE_H
#include "Parser.h"

namespace EU4
{
class EU4Empire: commonItems::parser
{
  public:
	explicit EU4Empire(std::istream& theStream);

	[[nodiscard]] const auto& getEmperor() const { return emperor; }

  private:
	void registerKeywords();

	std::string emperor;
};
} // namespace EU4

#endif // EU4EMPIRE_H