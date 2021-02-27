#ifndef UNIT_COLORS_H
#define UNIT_COLORS_H

#include "Parser.h"

namespace mappers
{
class UnitColors: commonItems::parser
{
  public:
	UnitColors();
	UnitColors(std::istream& theStream);

	[[nodiscard]] const auto& getUnitColorsMap() const { return unitColorsMap; }

  private:
	void registerKeys();
	std::map<std::string, std::string> unitColorsMap;
};
}; // namespace mappers

#endif // UNIT_COLORS_H
