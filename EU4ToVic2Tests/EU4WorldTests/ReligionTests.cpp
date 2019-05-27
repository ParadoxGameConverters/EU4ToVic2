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
#include "../EU4toV2/Source/EU4World/Religion.h"



TEST(EU4World_EU4ReligionTests, sameReligionReturnsSameReligion)
{
	EU4::Religion religionOne("religion", "group");
	EU4::Religion religionTwo("religion", "group");

	ASSERT_TRUE(religionOne.isSameReligion(religionTwo));
}


TEST(EU4World_EU4ReligionTests, sameReligionReturnsNotRelatedTo)
{
	EU4::Religion religionOne("religion", "group");
	EU4::Religion religionTwo("religion", "group");
	ASSERT_FALSE(religionOne.isRelatedTo(religionTwo));
}


TEST(EU4World_EU4ReligionTests, sameReligionReturnsNotInfidel)
{
	EU4::Religion religionOne("religion", "group");
	EU4::Religion religionTwo("religion", "group");

	ASSERT_FALSE(religionOne.isInfidelTo(religionTwo));
}


TEST(EU4World_EU4ReligionTests, differentReligionSameGroupReturnsNotSameReligion)
{
	EU4::Religion religionOne("religion", "group");
	EU4::Religion religionTwo("differentReligion", "group");

	ASSERT_FALSE(religionOne.isSameReligion(religionTwo));
}


TEST(EU4World_EU4ReligionTests, differentReligionSameGroupReturnsRelatedTo)
{
	EU4::Religion religionOne("religion", "group");
	EU4::Religion religionTwo("differentReligion", "group");
	ASSERT_TRUE(religionOne.isRelatedTo(religionTwo));
}


TEST(EU4World_EU4ReligionTests, differentReligionSameGroupReturnsNotInfidel)
{
	EU4::Religion religionOne("religion", "group");
	EU4::Religion religionTwo("differentReligion", "group");

	ASSERT_FALSE(religionOne.isInfidelTo(religionTwo));
}


TEST(EU4World_EU4ReligionTests, differentReligionDifferentGroupReturnsNotSameReligion)
{
	EU4::Religion religionOne("religion", "group");
	EU4::Religion religionTwo("differentReligion", "differentGroup");

	ASSERT_FALSE(religionOne.isSameReligion(religionTwo));
}


TEST(EU4World_EU4ReligionTests, differentReligionDifferentGroupReturnsNotRelatedTo)
{
	EU4::Religion religionOne("religion", "group");
	EU4::Religion religionTwo("differentReligion", "differentGroup");
	ASSERT_FALSE(religionOne.isRelatedTo(religionTwo));
}


TEST(EU4World_EU4ReligionTests, differentReligionDifferentGroupReturnsInfidel)
{
	EU4::Religion religionOne("religion", "group");
	EU4::Religion religionTwo("differentReligion", "differentGroup");

	ASSERT_TRUE(religionOne.isInfidelTo(religionTwo));
}


// same religion different group is not a possibility