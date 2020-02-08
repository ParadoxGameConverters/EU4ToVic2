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
#include "../EU4toV2/Source/EU4World/Provinces/ProvinceHistory.h"
#include "../EU4toV2/Source/EU4World/Religions/Religions.h"
#include <sstream>



TEST(EU4World_ProvinceHistoryTests, firstOwnedDateCanBeEmpty)
{
	std::stringstream input;
	input << "{}";

	EU4::ProvinceHistory theHistory(input);
	ASSERT_FALSE(theHistory.getFirstOwnedDate());
}


TEST(EU4World_ProvinceHistoryTests, firstOwnedDateCanBeSetFromStartingOwner)
{
	std::stringstream input;
	input << "{\n";
	input << "	owner=\"TAG\"\n";
	input << "}";

	EU4::ProvinceHistory theHistory(input);
	std::optional<date> firstOwnedDate = theHistory.getFirstOwnedDate();
	ASSERT_TRUE(firstOwnedDate);
	ASSERT_EQ(firstOwnedDate, date("1000.1.1"));
}


TEST(EU4World_ProvinceHistoryTests, firstOwnedDateCanBeSetFromOwnerChange)
{
	std::stringstream input;
	input << "{\n";
	input << "	1600.1.1={\n";
	input << "		owner=\"TAG\"\n";
	input << "	}\n";
	input << "}";

	EU4::ProvinceHistory theHistory(input);
	std::optional<date> firstOwnedDate = theHistory.getFirstOwnedDate();
	ASSERT_TRUE(firstOwnedDate);
	ASSERT_EQ(firstOwnedDate, date("1600.1.1"));
}


TEST(EU4World_ProvinceHistoryTests, hasOriginalCultureIfNoCulture)
{
	std::stringstream input;
	input << "{}";

	EU4::ProvinceHistory theHistory(input);
	ASSERT_TRUE(theHistory.hasOriginalCulture());
}


TEST(EU4World_ProvinceHistoryTests, hasOriginalCultureIfNoCultureChange)
{
	std::stringstream input;
	input << "{\n";
	input << "	culture=theCulture\n";
	input << "}";

	EU4::ProvinceHistory theHistory(input);
	ASSERT_TRUE(theHistory.hasOriginalCulture());
}


TEST(EU4World_ProvinceHistoryTests, hasNonOriginalCultureIfCultureChange)
{
	std::stringstream input;
	input << "{\n";
	input << "	culture=theCulture\n";
	input << "	1600.1.1={\n";
	input << "		culture=newCulture\n";
	input << "	}\n";
	input << "}";

	EU4::ProvinceHistory theHistory(input);
	ASSERT_FALSE(theHistory.hasOriginalCulture());
}


TEST(EU4World_ProvinceHistoryTests, hasOriginalCultureIfCultureChangesBack)
{
	std::stringstream input;
	input << "{\n";
	input << "	culture=theCulture\n";
	input << "	1600.1.1={\n";
	input << "		culture=newCulture\n";
	input << "	}\n";
	input << "	1700.1.1={\n";
	input << "		culture=theCulture\n";
	input << "	}\n";
	input << "}";

	EU4::ProvinceHistory theHistory(input);
	ASSERT_TRUE(theHistory.hasOriginalCulture());
}


TEST(EU4World_ProvinceHistoryTests, wasNotColonizedIfNoOwner)
{
	std::stringstream input;
	input << "={\n";
	input << "}";

	EU4::ProvinceHistory theHistory(input);
	ASSERT_FALSE(theHistory.wasColonized());
}


TEST(EU4World_ProvinceHistoryTests, wasNotColonizedIfHasOwnerAtStart)
{
	std::stringstream input;
	input << "={\n";
	input << "	owner=TAG\n";
	input << "}";

	EU4::ProvinceHistory theHistory(input);
	ASSERT_FALSE(theHistory.wasColonized());
}


TEST(EU4World_ProvinceHistoryTests, wasColonizedIfFirstOwnerIsNotAtStart)
{
	std::stringstream input;
	input << "={\n";
	input << "	culture=theCulture\n";
	input << "	1600.1.1={\n";
	input << "		owner=TAG\n";
	input << "		culture=newCulture\n";
	input << "	}\n";
	input << "}";

	EU4::ProvinceHistory theHistory(input);
	ASSERT_TRUE(theHistory.wasColonized());
}


TEST(EU4World_ProvinceHistoryTests, wasNotColonizedIfCultureDidntChange)
{
	std::stringstream input;
	input << "={\n";
	input << "	culture=theCulture\n";
	input << "	1600.1.1={\n";
	input << "		owner=TAG\n";
	input << "		culture=theCulture\n";
	input << "	}\n";
	input << "}";

	EU4::ProvinceHistory theHistory(input);
	ASSERT_FALSE(theHistory.wasColonized());
}

