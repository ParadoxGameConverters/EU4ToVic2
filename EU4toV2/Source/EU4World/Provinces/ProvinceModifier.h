#ifndef PROVINCE_MODIFIER_H
#define PROVINCE_MODIFIER_H
#include "Parser.h"
#include <string>

namespace EU4
{
class ProvinceModifier: commonItems::parser
{
  public:
	explicit ProvinceModifier(std::istream& theStream);

	[[nodiscard]] const auto& getModifier() const { return modifier; }

  private:
	void registerKeys();
	
	std::string modifier;
};
} // namespace EU4

#endif // PROVINCE_MODIFIER_H