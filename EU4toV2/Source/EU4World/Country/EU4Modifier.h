#ifndef EU4_MODIFIER_H
#define EU4_MODIFIER_H
#include "Parser.h"

namespace EU4
{
class EU4Modifier: commonItems::parser
{
  public:
	EU4Modifier() = default;
	explicit EU4Modifier(std::istream& theStream);

	[[nodiscard]] const auto& getModifier() const { return modifier; }

  private:
	void registerKeys();

	std::string modifier;
};
} // namespace EU4

#endif // EU4_MODIFIER_H