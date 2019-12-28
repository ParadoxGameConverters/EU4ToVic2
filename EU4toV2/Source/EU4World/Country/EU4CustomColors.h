#ifndef EU4_CUSTOMCOLORS_H_
#define EU4_CUSTOMCOLORS_H_


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
		CustomColorsBlock getCustomColors() const { return customColors; }

		void setFlag(int fl) { customColors.flag = fl; }
		void setColor(int col) { customColors.color = col; }
		void setSymbolIndex(int si) { customColors.symbolIndex = si; }
		void setFlagColors(commonItems::Color col) { customColors.flagColors = col; }

	private:
		CustomColorsBlock customColors;
	};
}



#endif // EU4_CUSTOMCOLORS_H_
