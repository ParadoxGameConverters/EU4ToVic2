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


#include "EU4NationalSymbol.h"
#include "Log.h"
#include "NewParserToOldParserConverters.h"
#include "ParserHelpers.h"
#include <algorithm>


EU4::CustomColors::CustomColors(std::istream& theStream)
{
	registerKeyword(std::regex("flag"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt flagInt(theStream);
			customColors.flag = flagInt.getInt() + 1;
		}
	);
	registerKeyword(std::regex("color"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt colorInt(theStream);
			customColors.color = colorInt.getInt() + 1;
		}
	);
	registerKeyword(std::regex("symbol_index"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt siInt(theStream);
			customColors.symbolIndex = siInt.getInt() + 1;
		}
	);
	registerKeyword(std::regex("flag_colors"), [this](const std::string& colorsString, std::istream& theStream)
		{
			customColors.flagColors = commonItems::Color(theStream);
		}
	);

	registerKeyword(std::regex("[a-z0-9\\_]+"), commonItems::ignoreItem);
	parseStream(theStream);
}

