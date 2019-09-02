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
#include "../EU4toV2/Source/EU4World/Provinces/EU4Province.h"
#include "../EU4toV2/Source/EU4World/Buildings/Buildings.h"
#include "../EU4toV2/Source/EU4World/Modifiers/Modifiers.h"
#include <sstream>



TEST(EU4World_ProvinceTests, numberProperlyInterpreted)
{
	std::stringstream input;
	input << "={}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-42", input, buildings, modifiers);
	ASSERT_EQ(theProvince.getNum(), 42);
}


TEST(EU4World_ProvinceTests, nameDefaultsToEmpty)
{
	std::stringstream input;
	input << "={}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_EQ(theProvince.getName(), "");
}


TEST(EU4World_ProvinceTests, nameCanBeSet)
{
	std::stringstream input;
	input << "={\n";
	input << "	name=\"theName\"\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_EQ(theProvince.getName(), "theName");
}


TEST(EU4World_ProvinceTests, ownerStringDefaultsToEmpty)
{
	std::stringstream input;
	input << "={}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_EQ(theProvince.getOwnerString(), "");
}


TEST(EU4World_ProvinceTests, ownerStringCanBeSet)
{
	std::stringstream input;
	input << "={\n";
	input << "	owner=\"TAG\"\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_EQ(theProvince.getOwnerString(), "TAG");
}


TEST(EU4World_ProvinceTests, coresDefaultToEmpty)
{
	std::stringstream input;
	input << "={}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_EQ(theProvince.getCores().size(), 0);
}


TEST(EU4World_ProvinceTests, coresCanBeSetWithNewStyle)
{
	std::stringstream input;
	input << "={\n";
	input << "	cores={\n";
	input << "		\"TAG\"\n";
	input << "	}\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_EQ(theProvince.getCores().count("TAG"), 1);
}


TEST(EU4World_ProvinceTests, coresCanBeSetWithOldStyle)
{
	std::stringstream input;
	input << "={\n";
	input << "	core=\"TAG\"\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_EQ(theProvince.getCores().count("TAG"), 1);
}


TEST(EU4World_ProvinceTests, coresCanBeAdded)
{
	std::stringstream input;
	input << "={}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	theProvince.addCore("TAG");

	ASSERT_EQ(theProvince.getCores().count("TAG"), 1);
}


TEST(EU4World_ProvinceTests, coresCanBeRemoved)
{
	std::stringstream input;
	input << "={\n";
	input << "	core=\"TAG\"\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	theProvince.removeCore("TAG");

	ASSERT_EQ(theProvince.getCores().count("TAG"), 0);
}


TEST(EU4World_ProvinceTests, inHreDefaultsToFalse)
{
	std::stringstream input;
	input << "={}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_FALSE(theProvince.inHre());
}


TEST(EU4World_ProvinceTests, inHreCanRemainNegative)
{
	std::stringstream input;
	input << "={\n";
	input << "	hre=no\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_FALSE(theProvince.inHre());
}


TEST(EU4World_ProvinceTests, inHreCanBeTrue)
{
	std::stringstream input;
	input << "={\n";
	input << "	hre=yes\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_TRUE(theProvince.inHre());
}


TEST(EU4World_ProvinceTests, isColonyDefaultsToFalse)
{
	std::stringstream input;
	input << "={}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_FALSE(theProvince.isColony());
}


TEST(EU4World_ProvinceTests, isColonyCanBeSet)
{
	std::stringstream input;
	input << "={\n";
	input << "	colonysize=800.000\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_TRUE(theProvince.isColony());
}


TEST(EU4World_ProvinceTests, wasColonizedDefaultsToFalse)
{
	std::stringstream input;
	input << "={}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_FALSE(theProvince.wasColonised());
}


TEST(EU4World_ProvinceTests, wasColonizedTrueIfHasOriginalColoniser)
{
	std::stringstream input;
	input << "={\n";
	input << "	original_coloniser=\"TAG\"\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_TRUE(theProvince.wasColonised());
}


TEST(EU4World_ProvinceTests, wasColonizedFalseIfOwnedAtStart)
{
	std::stringstream input;
	input << "={\n";
	input << "	owner=\"TAG\"\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_FALSE(theProvince.wasColonised());
}


TEST(EU4World_ProvinceTests, wasColonizedFalseIfHasOriginalCulture)
{
	std::stringstream input;
	input << "={\n";
	input << "	culture=theCulture\n";
	input << "	history={\n";
	input << "		1600.1.1={\n";
	input << "			owner=\"TAG\"\n";
	input << "		}\n";
	input << "	}\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_FALSE(theProvince.wasColonised());
}


TEST(EU4World_ProvinceTests, wasColonizedTrueIfCultureChanged)
{
	std::stringstream input;
	input << "={\n";
	input << "	culture=theCulture\n";
	input << "	history={\n";
	input << "		1600.1.1={\n";
	input << "			owner=\"TAG\"\n";
	input << "			culture=newCulture\n";
	input << "		}\n";
	input << "	}\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_FALSE(theProvince.wasColonised());
}


TEST(EU4World_ProvinceTests, popRatiosDefaultToEmpty)
{
	std::stringstream input;
	input << "={\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_EQ(theProvince.getPopRatios().size(), 0);
}


TEST(EU4World_ProvinceTests, popRatiosCanBeAdded)
{
	std::stringstream input;
	input << "={\n";
	input << "	history={\n";
	input << "		culture=theCulture\n";
	input << "		religion=theReligion\n";
	input << "	}\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_EQ(theProvince.getPopRatios().size(), 1);
}


TEST(EU4World_ProvinceTests, getFirstOwnedDateDefaultsToEmpty)
{
	std::stringstream input;
	input << "={\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_FALSE(theProvince.getFirstOwnedDate());
}


TEST(EU4World_ProvinceTests, getFirstOwnedDateCanDetectOwnershipAtStartDate)
{
	std::stringstream input;
	input << "={\n";
	input << "	history={\n";
	input << "		owner=\"TAG\"\n";
	input << "	}\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_EQ(*theProvince.getFirstOwnedDate(), date("1444.11.11"));
}


TEST(EU4World_ProvinceTests, getFirstOwnedDateCanDetectLaterOwnership)
{
	std::stringstream input;
	input << "={\n";
	input << "	history={\n";
	input << "		1600.1.1={\n";
	input << "			owner=\"TAG\"\n";
	input << "		}\n";
	input << "	}\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_EQ(theProvince.getFirstOwnedDate(), date("1600.1.1"));
}


TEST(EU4World_ProvinceTests, wasInfidelConquestDefaultsToFalse)
{
	std::stringstream input;
	input << "={\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Religions religions;

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_FALSE(theProvince.wasInfidelConquest("ownerReligion", religions));
}


TEST(EU4World_ProvinceTests, wasInfidelConquestCanBeTrue)
{
	std::stringstream input;
	input << "={\n";
	input << "	history={\n";
	input << "	{\n";
	input << "		religion=provinceReligion\n";
	input << "		culture=firstCulture\n";
	input << "		1600.1.1={\n";
	input << "			culture=newCulture\n";
	input << "		}\n";
	input << "	}";
	input << "}";

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

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_TRUE(theProvince.wasInfidelConquest("ownerReligion", theReligions));
}


TEST(EU4World_ProvinceTests, hasBuildingDefaultsToFalse)
{
	std::stringstream input;
	input << "={\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_FALSE(theProvince.hasBuilding("theBuilding"));
}


TEST(EU4World_ProvinceTests, hasBuildingCanBeTrue)
{
	std::stringstream input;
	input << "={\n";
	input << "	buildings={\n";
	input << "		theBuilding=yes\n";
	input << "	}\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_TRUE(theProvince.hasBuilding("theBuilding"));
}

TEST(EU4World_ProvinceTests, hasGreatProjectDefaultsToFalse)
{
	std::stringstream input;
	input << "={\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_FALSE(theProvince.hasGreatProject("test_canal"));
}

TEST(EU4World_ProvinceTests, hasGreatProjectFindsProjects)
{
	std::stringstream input;
	input << "={\n";
	input << "	great_projects={\n";
	input << "		\"test_canal\"\n";
	input << "	}\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_TRUE(theProvince.hasGreatProject("test_canal"));
}


TEST(EU4World_ProvinceTests, hasModifierDefaultsToFalse)
{
	std::stringstream input;
	input << "={\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_FALSE(theProvince.hasModifier("center_of_trade_modifier"));
}

TEST(EU4World_ProvinceTests, hasModifierFindsModifiers)
{
	std::stringstream input;
	input << "={\n";
	input << "\tmodifier={\n";
	input << "\t\tmodifier=\"center_of_trade_modifier\"\n";
	input << "\t}\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_TRUE(theProvince.hasModifier("center_of_trade_modifier"));
}


TEST(EU4World_ProvinceTests, getCulturePercentDefaultsToZero)
{
	std::stringstream input;
	input << "={\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_EQ(theProvince.getCulturePercent("theCulture"), 0.0);
}


TEST(EU4World_ProvinceTests, getCulturePercentCanFullyMatch)
{
	std::stringstream input;
	input << "={\n";
	input << "	history={\n";
	input << "		culture = theCulture\n";
	input << "		religion=theReligion\n";
	input << "	}\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_EQ(theProvince.getCulturePercent("theCulture"), 1.0);
}


TEST(EU4World_ProvinceTests, getCulturePercentDeterminedByHistory)
{
	std::stringstream input;
	input << "={\n";
	input << "	history={\n";
	input << "		culture=theCulture\n";
	input << "		religion=theReligion\n";
	input << "		1621.1.3={\n";
	input << "			culture=newCulture\n";
	input << "		}\n";
	input << "	}\n";
	input << "}";

	std::stringstream buildingsInput;
	EU4::Buildings buildings(buildingsInput);

	std::stringstream modifiersInput;
	EU4::Modifiers modifiers(modifiersInput);

	EU4::Province theProvince("-1", input, buildings, modifiers);
	ASSERT_EQ(theProvince.getCulturePercent("theCulture"), 0.5);
}