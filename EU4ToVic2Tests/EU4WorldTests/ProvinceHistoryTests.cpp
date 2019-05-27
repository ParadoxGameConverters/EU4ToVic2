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
#include "../EU4toV2/Source/EU4World/ProvinceHistory.h"
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
	ASSERT_EQ(firstOwnedDate, date("1444.11.11"));
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


TEST(EU4World_ProvinceHistoryTests, wasNotInfidelConquestIfNoReligion)
{
	std::stringstream input;
	input << "{}";
	EU4::ProvinceHistory theHistory(input);

	EU4::Religions theReligions;

	ASSERT_FALSE(theHistory.wasInfidelConquest(theReligions, "ownerReligion", false, 1));
}


TEST(EU4World_ProvinceHistoryTests, wasInfidelConquestIfProvinceReligionNotHandled)
{
	std::stringstream input;
	input << "{\n";
	input << "	religion=provinceReligion\n";
	input << "}";
	EU4::ProvinceHistory theHistory(input);

	EU4::Religions theReligions;
	std::stringstream religionsInput;
	religionsInput << "={\n";
	religionsInput << "	religionGroup={\n";
	religionsInput << "		ownerReligion={}\n";
	religionsInput << "	}\n";
	religionsInput << "}";
	theReligions.addReligions(religionsInput);

	ASSERT_TRUE(theHistory.wasInfidelConquest(theReligions, "ownerReligion", false, 1));
}


TEST(EU4World_ProvinceHistoryTests, wasInfidelConquestIfOwnerReligionNotHandled)
{
	std::stringstream input;
	input << "{\n";
	input << "	religion=provinceReligion\n";
	input << "}";
	EU4::ProvinceHistory theHistory(input);

	EU4::Religions theReligions;
	std::stringstream religionsInput;
	religionsInput << "={\n";
	religionsInput << "	religionGroup={\n";
	religionsInput << "		provinceReligion={}\n";
	religionsInput << "	}\n";
	religionsInput << "}";
	theReligions.addReligions(religionsInput);

	ASSERT_TRUE(theHistory.wasInfidelConquest(theReligions, "ownerReligion", false, 1));
}


TEST(EU4World_ProvinceHistoryTests, wasInfidelConquestIfReligionsAreMutualInfidels)
{
	std::stringstream input;
	input << "{\n";
	input << "	religion=provinceReligion\n";
	input << "	culture=firstCulture\n";
	input << "	1600.1.1={\n";
	input << "		culture=newCulture\n";
	input << "	}\n";
	input << "}";
	EU4::ProvinceHistory theHistory(input);

	EU4::Religions theReligions;
	std::stringstream religionsInput;
	religionsInput << "={\n";
	religionsInput << "	religionGroup={\n";
	religionsInput << "		provinceReligion={}\n";
	religionsInput << "	}\n";
	religionsInput << "	otherReligionGroup={\n";
	religionsInput << "		ownerReligion={}\n";
	religionsInput << "	}\n";
	religionsInput << "}";
	theReligions.addReligions(religionsInput);

	ASSERT_TRUE(theHistory.wasInfidelConquest(theReligions, "ownerReligion", false, 1));
}


TEST(EU4World_ProvinceHistoryTests, wasNotInfidelConquestIfReligionsAreSame)
{
	std::stringstream input;
	input << "{\n";
	input << "	religion=religion\n";
	input << "	culture=firstCulture\n";
	input << "	1600.1.1={\n";
	input << "		culture=newCulture\n";
	input << "	}\n";
	input << "}";
	EU4::ProvinceHistory theHistory(input);

	EU4::Religions theReligions;
	std::stringstream religionsInput;
	religionsInput << "={\n";
	religionsInput << "	religionGroup={\n";
	religionsInput << "		religion={}\n";
	religionsInput << "		religion={}\n";
	religionsInput << "	}\n";
	religionsInput << "}";
	theReligions.addReligions(religionsInput);

	ASSERT_FALSE(theHistory.wasInfidelConquest(theReligions, "religion", false, 1));
}


TEST(EU4World_ProvinceHistoryTests, wasNotInfidelConquestIfReligionsAreInSameGroup)
{
	std::stringstream input;
	input << "{\n";
	input << "	religion=provinceReligion\n";
	input << "	culture=firstCulture\n";
	input << "	1600.1.1={\n";
	input << "		culture=newCulture\n";
	input << "	}\n";
	input << "}";
	EU4::ProvinceHistory theHistory(input);

	EU4::Religions theReligions;
	std::stringstream religionsInput;
	religionsInput << "={\n";
	religionsInput << "	religionGroup={\n";
	religionsInput << "		provinceReligion={}\n";
	religionsInput << "		ownerReligion={}\n";
	religionsInput << "	}\n";
	religionsInput << "}";
	theReligions.addReligions(religionsInput);

	ASSERT_FALSE(theHistory.wasInfidelConquest(theReligions, "ownerReligion", false, 1));
}


TEST(EU4World_ProvinceHistoryTests, wasNotInfidelConquestIfCultureDidntChange)
{
	std::stringstream input;
	input << "{\n";
	input << "	religion=provinceReligion\n";
	input << "	culture=firstCulture\n";
	input << "}";
	EU4::ProvinceHistory theHistory(input);

	EU4::Religions theReligions;
	std::stringstream religionsInput;
	religionsInput << "={\n";
	religionsInput << "	religionGroup={\n";
	religionsInput << "		provinceReligion={}\n";
	religionsInput << "	}\n";
	religionsInput << "	otherReligionGroup={\n";
	religionsInput << "		ownerReligion={}\n";
	religionsInput << "	}\n";
	religionsInput << "}";
	theReligions.addReligions(religionsInput);

	ASSERT_FALSE(theHistory.wasInfidelConquest(theReligions, "ownerReligion", false, 1));
}


TEST(EU4World_ProvinceHistoryTests, wasNotInfidelConquestIfProvinceWasColonized)
{
	std::stringstream input;
	input << "{\n";
	input << "	religion=provinceReligion\n";
	input << "	culture=firstCulture\n";
	input << "	1600.1.1={\n";
	input << "		culture=newCulture\n";
	input << "	}\n";
	input << "}";
	EU4::ProvinceHistory theHistory(input);

	EU4::Religions theReligions;
	std::stringstream religionsInput;
	religionsInput << "={\n";
	religionsInput << "	religionGroup={\n";
	religionsInput << "		provinceReligion={}\n";
	religionsInput << "	}\n";
	religionsInput << "	otherReligionGroup={\n";
	religionsInput << "		ownerReligion={}\n";
	religionsInput << "	}\n";
	religionsInput << "}";
	theReligions.addReligions(religionsInput);

	ASSERT_FALSE(theHistory.wasInfidelConquest(theReligions, "ownerReligion", true, 1));
}


TEST(EU4World_ProvinceHistoryTests, noCultureOrReligionChangeGivesEasyPopRatios)
{
	std::stringstream input;
	input << "{\n";
	input << "	culture=theCulture\n";
	input << "	religion=theReligion\n";
	input << "}";
	EU4::ProvinceHistory theHistory(input);

	std::vector<EU4::PopRatio> ratios = theHistory.getPopRatios();

	ASSERT_EQ(ratios.size(), 1);
	ASSERT_EQ(ratios[0].getCulture(), "theCulture");
	ASSERT_EQ(ratios[0].getReligion(), "theReligion");
	ASSERT_EQ(ratios[0].getUpperRatio(), 1.0);
	ASSERT_EQ(ratios[0].getMiddleRatio(), 1.0);
	ASSERT_EQ(ratios[0].getLowerRatio(), 1.0);
}


TEST(EU4World_ProvinceHistoryTests, endCultureOrReligionChangeGivesSimpleChangedRatios)
{
	std::stringstream input;
	input << "{\n";
	input << "	culture=theCulture\n";
	input << "	religion=theReligion\n";
	input << "	1836.1.1={\n";
	input << "		culture=newCulture\n";
	input << "		religion=newReligion\n";
	input << "	}\n";
	input << "}";
	EU4::ProvinceHistory theHistory(input);

	std::vector<EU4::PopRatio> ratios = theHistory.getPopRatios();

	ASSERT_EQ(ratios.size(), 2);
	ASSERT_EQ(ratios[0].getCulture(), "theCulture");
	ASSERT_EQ(ratios[0].getReligion(), "theReligion");
	ASSERT_EQ(ratios[0].getUpperRatio(), 0.5);
	ASSERT_EQ(ratios[0].getMiddleRatio(), 0.5);
	ASSERT_EQ(ratios[0].getLowerRatio(), 1.0);
	ASSERT_EQ(ratios[1].getCulture(), "newCulture");
	ASSERT_EQ(ratios[1].getReligion(), "newReligion");
	ASSERT_EQ(ratios[1].getUpperRatio(), 0.5);
	ASSERT_EQ(ratios[1].getMiddleRatio(), 0.5);
	ASSERT_EQ(ratios[1].getLowerRatio(), 0.0);
}


TEST(EU4World_ProvinceHistoryTests, pastCultureOrReligionChangeGivesDecayedRatios)
{
	std::stringstream input;
	input << "{\n";
	input << "	culture=theCulture\n";
	input << "	religion=theReligion\n";
	input << "	1736.1.1={\n";
	input << "		culture=newCulture\n";
	input << "		religion=newReligion\n";
	input << "	}\n";
	input << "}";
	EU4::ProvinceHistory theHistory(input);

	std::vector<EU4::PopRatio> ratios = theHistory.getPopRatios();

	ASSERT_EQ(ratios.size(), 2);
	ASSERT_EQ(ratios[0].getCulture(), "theCulture");
	ASSERT_EQ(ratios[0].getReligion(), "theReligion");
	ASSERT_EQ(ratios[0].getUpperRatio(), 0.25);
	ASSERT_EQ(ratios[0].getMiddleRatio(), 0.25);
	ASSERT_EQ(ratios[0].getLowerRatio(), 0.75);
	ASSERT_EQ(ratios[1].getCulture(), "newCulture");
	ASSERT_EQ(ratios[1].getReligion(), "newReligion");
	ASSERT_EQ(ratios[1].getUpperRatio(), 0.75);
	ASSERT_EQ(ratios[1].getMiddleRatio(), 0.75);
	ASSERT_EQ(ratios[1].getLowerRatio(), 0.25);
}


TEST(EU4World_ProvinceHistoryTests, multipleCultureOrReligionChangeGivesComplexRatios)
{
	std::stringstream input;
	input << "{\n";
	input << "	culture=theCulture\n";
	input << "	religion=theReligion\n";
	input << "	1736.1.1={\n";
	input << "		culture=newCulture\n";
	input << "		religion=newReligion\n";
	input << "	}\n";
	input << "	1836.1.1={\n";
	input << "		culture=newerCulture\n";
	input << "		religion=newerReligion\n";
	input << "	}\n";
	input << "}";
	EU4::ProvinceHistory theHistory(input);

	std::vector<EU4::PopRatio> ratios = theHistory.getPopRatios();

	ASSERT_EQ(ratios.size(), 3);
	ASSERT_EQ(ratios[0].getCulture(), "theCulture");
	ASSERT_EQ(ratios[0].getReligion(), "theReligion");
	ASSERT_EQ(ratios[0].getUpperRatio(), 0.125);
	ASSERT_EQ(ratios[0].getMiddleRatio(), 0.125);
	ASSERT_EQ(ratios[0].getLowerRatio(), 0.75);
	ASSERT_EQ(ratios[1].getCulture(), "newCulture");
	ASSERT_EQ(ratios[1].getReligion(), "newReligion");
	ASSERT_EQ(ratios[1].getUpperRatio(), 0.375);
	ASSERT_EQ(ratios[1].getMiddleRatio(), 0.375);
	ASSERT_EQ(ratios[1].getLowerRatio(), 0.25);
	ASSERT_EQ(ratios[2].getCulture(), "newerCulture");
	ASSERT_EQ(ratios[2].getReligion(), "newerReligion");
	ASSERT_EQ(ratios[2].getUpperRatio(), 0.5);
	ASSERT_EQ(ratios[2].getMiddleRatio(), 0.5);
	ASSERT_EQ(ratios[2].getLowerRatio(), 0.0);
}