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
#include "../EU4toV2/Source/EU4World/Provinces/DateItems.h"
#include "../common_items/Date.h"
#include <sstream>



TEST(EU4World_DateItemsTests, emptyItemsImported)
{
	std::stringstream input;
	input << "={\n";
	input << "}";

	EU4::DateItems theDateItems("1444.11.12", input);
	std::vector<EU4::DateItem> theItems = theDateItems.getItems();

	ASSERT_EQ(theItems.size(), 0);
}


TEST(EU4World_DateItemsTests, singleItemImported)
{
	std::stringstream input;
	input << "={\n";
	input << "	owner = \"SWE\"\n";
	input << "}";

	EU4::DateItems theDateItems("1444.11.12", input);
	std::vector<EU4::DateItem> theItems = theDateItems.getItems();

	ASSERT_EQ(theItems.size(), 1);
	ASSERT_EQ(theItems[0].getDate(), date("1444.11.12"));
	ASSERT_EQ(theItems[0].getType(), EU4::DateItemType::OWNER_CHANGE);
	ASSERT_EQ(theItems[0].getData(), "SWE");
}


TEST(EU4World_DateItemsTests, multipleItemsImported)
{
	std::stringstream input;
	input << "={\n";
	input << "	revolt = { \n";
	input << "		type = \"pretender_rebels\"\n";
	input << "		leader = \"Karl Knutsson Bonde\"\n";
	input << "		size = 1\n";
	input << "	}\n";
	input << "	controller = { \n";
	input << "		tag = \"REB\"\n";
	input << "	}\n";
	input << "}";

	EU4::DateItems theDateItems("1438.10.1", input);
	std::vector<EU4::DateItem> theItems = theDateItems.getItems();

	ASSERT_EQ(theItems.size(), 2);
	ASSERT_EQ(theItems[0].getDate(), date("1438.10.1"));
	ASSERT_EQ(theItems[0].getType(), EU4::DateItemType::OTHER_CHANGE);
	ASSERT_EQ(theItems[0].getData(), "");
	ASSERT_EQ(theItems[1].getDate(), date("1438.10.1"));
	ASSERT_EQ(theItems[1].getType(), EU4::DateItemType::OTHER_CHANGE);
	ASSERT_EQ(theItems[1].getData(), "");
}