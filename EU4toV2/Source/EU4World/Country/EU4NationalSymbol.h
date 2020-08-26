#ifndef EU4_NATIONAL_SYMBOL_H
#define EU4_NATIONAL_SYMBOL_H

#include "EU4CustomColors.h"

namespace EU4
{
class NationalSymbol: public commonItems::parser
{
  public:
	NationalSymbol() = default;
	explicit NationalSymbol(std::istream& theStream);
	[[nodiscard]] const auto& getMapColor() const { return mapColor; }
	[[nodiscard]] const auto& getCountryColor() const { return countryColor; }
	[[nodiscard]] const auto& getRevolutionaryColor() const { return revolutionaryColor; }
	auto& getCustomColors() { return customColors; }
	[[nodiscard]] auto isCustomColorsInitialized() const { return customColorsInitialized; }

	void setMapColor(const commonItems::newColor& col) { mapColor = col; }
	void setCountryColor(const commonItems::newColor& col) { countryColor = col; }
	void setRevolutionaryColor(const commonItems::newColor& col) { revolutionaryColor = col; }
	void setCustomColors(const CustomColors& col) { customColors = col; }
	void setCustomColorsInitialized() { customColorsInitialized = true; }

  private:
	CustomColors customColors;
	commonItems::newColor mapColor;
	commonItems::newColor countryColor;
	commonItems::newColor revolutionaryColor;
	bool customColorsInitialized = false;
};
} // namespace EU4

#endif // EU4_NATIONAL_SYMBOL_H
