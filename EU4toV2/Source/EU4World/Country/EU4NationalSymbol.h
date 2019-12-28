#ifndef EU4_NATIONALSYMBOL_H_
#define EU4_NATIONALSYMBOL_H_

#include "EU4CustomColors.h"


namespace EU4
{
	class NationalSymbol: public commonItems::parser
	{
	public:
		NationalSymbol() = default;
		NationalSymbol(std::istream& theStream);
		commonItems::Color getMapColor() const { return mapColor; }
		commonItems::Color getCountryColor() const { return countryColor; }
		commonItems::Color getRevolutionaryColor() const { return revolutionaryColor; }
		CustomColors& retrieveCustomColors() { return customColors; }
		CustomColors getCustomColors() const { return customColors; }
		bool isCustomColorsInitialized() const { return customColorsInitialized; }

		void setMapColor(commonItems::Color col) { mapColor = col; }
		void setCountryColor(commonItems::Color col) { countryColor = col; }
		void setRevolutionaryColor(commonItems::Color col) { revolutionaryColor = col; }
		void setCustomColors(CustomColors col) { customColors = col; }
		void setCustomColorsInitialized() { customColorsInitialized = true; }

	private:
		CustomColors customColors;
		commonItems::Color mapColor = commonItems::Color(0, 0, 0);
		commonItems::Color countryColor = commonItems::Color(0, 0, 0);
		commonItems::Color revolutionaryColor = commonItems::Color(0, 0, 0);
		bool customColorsInitialized = false;
	};
}



#endif // EU4_NATIONALSYMBOL_H_
