/*Copyright (c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/



#include "gtest/gtest.h"
#include "../EU4toV2/Source/EU4World/Regions/Area.h"
#include <sstream>



TEST(EU4World_AreaTests, emptyAreaHasNoProvinces)
{
	std::stringstream input;
	input << "{}";

	EU4::Area theArea(input);
	ASSERT_EQ(theArea.getProvinces().size(), 0);
}


TEST(EU4World_AreaTests, provinceCanBeRead)
{
	std::stringstream input;
	input << "{ 56 }";

	EU4::Area theArea(input);
	ASSERT_EQ(theArea.getProvinces().count(56), 1);
}


TEST(EU4World_AreaTests, colorAreaWorks)
{
	std::stringstream input;
	input << "{\n";
	input << "\tcolor = { 1 2 3 }\n";
	input << "\t56\n";
	input << "}";

	EU4::Area theArea(input);
	ASSERT_EQ(theArea.getProvinces().count(3), 0);
}