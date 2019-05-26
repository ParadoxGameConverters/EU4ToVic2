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
#include "../EU4toV2/Source/EU4World/DateItem.h"
#include "../common_items/Date.h"
#include <sstream>



TEST(EU4World_EU4DateItemTests, datecanBeSet)
{
	std::stringstream input;
	input << "=bar";

	EU4::DateItem theDateItem("2019.5.26", "foo", input);
	ASSERT_EQ(theDateItem.getDate(), date("2019.5.26"));
}


TEST(EU4World_EU4DateItemTests, ownerChangeImports)
{
	std::stringstream input;
	input << "=\"SWE\"";

	EU4::DateItem theDateItem("1444.11.12", "owner", input);
	ASSERT_EQ(theDateItem.getType(), EU4::DateItemType::OWNER_CHANGE);
	ASSERT_EQ(theDateItem.getData(), "SWE");
}


TEST(EU4World_EU4DateItemTests, cultureChangeImports)
{
	std::stringstream input;
	input << "=polish";

	EU4::DateItem theDateItem("1444.11.12", "culture", input);
	ASSERT_EQ(theDateItem.getType(), EU4::DateItemType::CULTURE_CHANGE);
	ASSERT_EQ(theDateItem.getData(), "polish");
}


TEST(EU4World_EU4DateItemTests, religionChangeImports)
{
	std::stringstream input;
	input << "=protestant";

	EU4::DateItem theDateItem("1444.11.12", "religion", input);
	ASSERT_EQ(theDateItem.getType(), EU4::DateItemType::RELIGION_CHANGE);
	ASSERT_EQ(theDateItem.getData(), "protestant");
}


TEST(EU4World_EU4DateItemTests, otherChangesImport)
{
	std::stringstream input;
	input << " = { \n";
	input << "	tag = \"REB\"\n";
	input << "}\n";

	EU4::DateItem theDateItem("1444.11.12", "controller", input);
	ASSERT_EQ(theDateItem.getType(), EU4::DateItemType::OTHER_CHANGE);
	ASSERT_EQ(theDateItem.getData(), "");
}