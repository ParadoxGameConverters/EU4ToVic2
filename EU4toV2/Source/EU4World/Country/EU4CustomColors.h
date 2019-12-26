/*Copyright(c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */


#ifndef EU4_CUSTOMCOLORS_H_
#define EU4_CUSTOMCOLORS_H_


#include "Color.h"
#include "Date.h"
#include <memory>
#include <optional>
#include <set>


namespace EU4
{
	struct CustomColorsBlock
	{
		int flag = -1;
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
