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
		int flag = 1;
		int color = 1;
		int symbolIndex = 1;
		commonItems::Color flagColours = commonItems::Color(255, 255, 255);
	};

	class CustomColors : public  commonItems::parser
	{
	public:
		CustomColors(std::istream& theStream);
		CustomColorsBlock getCustomColors() const { return customColors; }
	private:
		CustomColorsBlock customColors;
	};
}



#endif // EU4_CUSTOMCOLORS_H_
