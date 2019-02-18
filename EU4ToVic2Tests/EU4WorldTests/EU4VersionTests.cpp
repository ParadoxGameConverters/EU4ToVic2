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



TEST(EU4World_EU4VersionTests, FourthPartCanDistinguish)
{
	EU4::Version lower("0.0.0.1");
	EU4::Version upper("0.0.0.2");
	ASSERT_GE(upper, lower);
}


TEST(EU4World_EU4VersionTests, ThirdPartCanDistinguish)
{
	EU4::Version lower("0.0.1.0");
	EU4::Version upper("0.0.2.0");
	ASSERT_GE(upper, lower);
}

TEST(EU4World_EU4VersionTests, ThirdPartHasPriorityOverFourthPart)
{
	EU4::Version lower("0.0.1.2");
	EU4::Version upper("0.0.2.1");
	ASSERT_GE(upper, lower);
}


TEST(EU4World_EU4VersionTests, SecondPartCanDistinguish)
{
	EU4::Version lower("0.1.0.0");
	EU4::Version upper("0.2.0.0");
	ASSERT_GE(upper, lower);
}


TEST(EU4World_EU4VersionTests, SecondPartHasPriorityOverThirdPart)
{
	EU4::Version lower("0.1.2.0");
	EU4::Version upper("0.2.1.0");
	ASSERT_GE(upper, lower);
}


TEST(EU4World_EU4VersionTests, FirstPartCanDistinguish)
{
	EU4::Version lower("1.0.0.0");
	EU4::Version upper("2.0.0.0");
	ASSERT_GE(upper, lower);
}


TEST(EU4World_EU4VersionTests, FirstPartHasPriorityOverSecondPart)
{
	EU4::Version lower("1.2.0.0");
	EU4::Version upper("2.1.0.0");
	ASSERT_GE(upper, lower);
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