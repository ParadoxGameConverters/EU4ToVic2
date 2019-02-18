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
#include "../EU4toV2/Source/EU4World/EU4Version.h"



TEST(EU4World_EU4VersionTests, GreaterThanOrEqualsFindsGreaterThanFromFourthPart)
{
	EU4::Version versionOne("1.2.3.5");
	EU4::Version versionTwo("1.2.3.4");
	ASSERT_GE(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, GreaterThanOrEqualsFindsGreaterThanFromThirdPart)
{
	EU4::Version versionOne("1.2.4.3");
	EU4::Version versionTwo("1.2.3.4");
	ASSERT_GE(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, GreaterThanOrEqualsFindsGreaterThanFromSecondPart)
{
	EU4::Version versionOne("1.3.2.3");
	EU4::Version versionTwo("1.2.3.4");
	ASSERT_GE(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, GreaterThanOrEqualsFindsGreaterThanFromFirstPart)
{
	EU4::Version versionOne("2.1.2.3");
	EU4::Version versionTwo("1.2.3.4");
	ASSERT_GE(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, GreaterThanOrEqualsFindsEquality)
{
	EU4::Version versionOne("1.2.3.4");
	EU4::Version versionTwo("1.2.3.4");
	ASSERT_GE(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, GreaterThanFindsGreaterThanFromFourthPart)
{
	EU4::Version versionOne("1.2.3.5");
	EU4::Version versionTwo("1.2.3.4");
	ASSERT_GT(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, GreaterThanFindsGreaterThanFromThirdPart)
{
	EU4::Version versionOne("1.2.4.3");
	EU4::Version versionTwo("1.2.3.4");
	ASSERT_GT(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, GreaterThanFindsGreaterThanFromSecondPart)
{
	EU4::Version versionOne("1.3.2.3");
	EU4::Version versionTwo("1.2.3.4");
	ASSERT_GT(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, GreaterThanFindsGreaterThanFromFirstPart)
{
	EU4::Version versionOne("2.1.2.3");
	EU4::Version versionTwo("1.2.3.4");
	ASSERT_GT(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, LessThanFindsGreaterThanFromFourthPart)
{
	EU4::Version versionOne("1.2.3.4");
	EU4::Version versionTwo("1.2.3.5");
	ASSERT_LT(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, LessThanFindsGreaterThanFromThirdPart)
{
	EU4::Version versionOne("1.2.3.4");
	EU4::Version versionTwo("1.2.4.3");
	ASSERT_LT(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, LessThanFindsGreaterThanFromSecondPart)
{
	EU4::Version versionOne("1.2.3.4");
	EU4::Version versionTwo("1.3.2.3");
	ASSERT_LT(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, LessThanFindsGreaterThanFromFirstPart)
{
	EU4::Version versionOne("1.2.3.4");
	EU4::Version versionTwo("2.1.2.3");
	ASSERT_LT(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, LessThanOrEqualsFindsGreaterThanFromFourthPart)
{
	EU4::Version versionOne("1.2.3.4");
	EU4::Version versionTwo("1.2.3.5");
	ASSERT_LE(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, LessThanOrEqualsFindsGreaterThanFromThirdPart)
{
	EU4::Version versionOne("1.2.3.4");
	EU4::Version versionTwo("1.2.4.3");
	ASSERT_LE(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, LessThanOrEqualsFindsGreaterThanFromSecondPart)
{
	EU4::Version versionOne("1.2.3.4");
	EU4::Version versionTwo("1.3.2.3");
	ASSERT_LE(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, LessThanOrEqualsFindsGreaterThanFromFirstPart)
{
	EU4::Version versionOne("1.2.3.4");
	EU4::Version versionTwo("2.1.2.3");
	ASSERT_LE(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, LessThanOrEqualsFindsEquality)
{
	EU4::Version versionOne("1.2.3.4");
	EU4::Version versionTwo("1.2.3.4");
	ASSERT_LE(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, EqualityFindEquality)
{
	EU4::Version versionOne("1.2.3.4");
	EU4::Version versionTwo("1.2.3.4");
	ASSERT_EQ(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, InequalityFindInequalityFromFourthPart)
{
	EU4::Version versionOne("1.2.3.4");
	EU4::Version versionTwo("1.2.3.5");
	ASSERT_NE(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, InequalityFindInequalityFromThirdPart)
{
	EU4::Version versionOne("1.2.3.4");
	EU4::Version versionTwo("1.2.4.4");
	ASSERT_NE(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, InequalityFindInequalityFromSecondPart)
{
	EU4::Version versionOne("1.2.3.4");
	EU4::Version versionTwo("1.3.3.4");
	ASSERT_NE(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, InequalityFindInequalityFromFirstPart)
{
	EU4::Version versionOne("1.2.3.4");
	EU4::Version versionTwo("2.2.3.4");
	ASSERT_NE(versionOne, versionTwo);
}


TEST(EU4World_EU4VersionTests, DefaultIsAllZeros)
{
	EU4::Version allZeros("0.0.0.0");
	EU4::Version defaultVersion;
	ASSERT_EQ(allZeros, defaultVersion);
}


TEST(EU4World_EU4VersionTests, CanParseVersion)
{
	std::stringstream input("={\
		first=1\
		second=2\
		third=3\
		forth=4\
		name=\"Persia\"\
	}");
	EU4::Version streamVersion(input);
	ASSERT_EQ(EU4::Version("1.2.3.4"), streamVersion);
}