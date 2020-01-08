#ifndef EU4_CUSTOMCOLORS_H
#define EU4_CUSTOMCOLORS_H

#include "Color.h"

namespace EU4
{
	struct CustomColorsBlock
	{
		int flag = 0;
		int color = 0;
		int symbolIndex = 0;
		commonItems::Color flagColors = commonItems::Color(0, 0, 0);
	};

	class CustomColors : public  commonItems::parser
	{
	public:
		CustomColors() = default;
		CustomColors(std::istream& theStream);
		const CustomColorsBlock& getCustomColors() const { return customColors; }

		void setFlag(int fl) { customColors.flag = fl; }
		void setColor(int col) { customColors.color = col; }
		void setSymbolIndex(int si) { customColors.symbolIndex = si; }
		void setFlagColors(commonItems::Color col) { customColors.flagColors = col; }

	private:
		CustomColorsBlock customColors; // It walks like a color, it quacks like a color, but it's in fact a color INDEX from 00_custom_colors.txt
	};
}

#endif // EU4_CUSTOMCOLORS_H
