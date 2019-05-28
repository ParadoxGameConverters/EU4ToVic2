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
#include "../EU4toV2/Source/EU4World/Buildings.h"
#include <sstream>



TEST(EU4World_BuildingsTests, nonMatchingBuildingReturnsFalse)
{
	std::stringstream input;
	input << "={}";

	EU4::Buildings theBuildings(input);
	ASSERT_FALSE(theBuildings.hasBuilding("theBuilding"));
}


TEST(EU4World_BuildingsTests, matchingBuildingReturnsTrue)
{
	std::stringstream input;
	input << "={\n";
	input << "	theBuilding=yes\n";
	input << "}";

	EU4::Buildings theBuildings(input);
	ASSERT_TRUE(theBuildings.hasBuilding("theBuilding"));
}