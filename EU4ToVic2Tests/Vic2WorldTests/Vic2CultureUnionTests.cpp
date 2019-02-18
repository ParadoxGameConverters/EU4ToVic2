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
#include "../EU4toV2/Source/V2World/Vic2CultureUnion.h"



TEST(Vic2World_CultureUnionTests, defaultCultureIsBlank)
{
	std::stringstream input("");
	Vic2::CultureUnion theCultureUnion(input);

	ASSERT_EQ(theCultureUnion.takeTheUnion().first, "");
}


TEST(Vic2World_CultureUnionTests, cultureIsImported)
{
	std::stringstream input("culture = a_culture");
	Vic2::CultureUnion theCultureUnion(input);

	ASSERT_EQ(theCultureUnion.takeTheUnion().first, "a_culture");
}


TEST(Vic2World_CultureUnionTests, defaultNoTags)
{
	std::stringstream input("");
	Vic2::CultureUnion theCultureUnion(input);

	ASSERT_EQ(theCultureUnion.takeTheUnion().second.size(), 0);
}


TEST(Vic2World_CultureUnionTests, tagsAreImported)
{
	std::stringstream input("tag = TAG tag = TG2");
	Vic2::CultureUnion theCultureUnion(input);

	auto theUnion = theCultureUnion.takeTheUnion();
	ASSERT_EQ(theUnion.second[0], "TAG");
	ASSERT_EQ(theUnion.second[1], "TG2");
}