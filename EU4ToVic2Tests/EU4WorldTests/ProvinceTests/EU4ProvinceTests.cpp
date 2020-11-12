#include "Provinces/EU4Province.h"
#include "gtest/gtest.h"

TEST(EU4World_Province_EU4ProvinceTests, numberProperlyInterpreted)
{
	std::stringstream input;
	const EU4::Province theProvince("-42", input);

	ASSERT_EQ(42, theProvince.getNum());
}

TEST(EU4World_ProvinceTests, primitivesDefaultToDefaults)
{
	std::stringstream input;
	const EU4::Province theProvince("-1", input);

	ASSERT_TRUE(theProvince.getName().empty());
	ASSERT_TRUE(theProvince.getOwnerString().empty());
	ASSERT_TRUE(theProvince.getCores().empty());
	ASSERT_FALSE(theProvince.inHre());
	ASSERT_FALSE(theProvince.isColony());
	ASSERT_TRUE(theProvince.getPopRatios().empty());
	ASSERT_FALSE(theProvince.getFirstOwnedDate());
}

TEST(EU4World_ProvinceTests, primitivesCanBeSet)
{
	std::stringstream input;
	input << "name = theName\n";
	input << "owner = TAG\n";
	input << "colonysize = 800.000\n";
	const EU4::Province theProvince("-1", input);

	ASSERT_EQ("theName", theProvince.getName());
	ASSERT_EQ("TAG", theProvince.getOwnerString());
	ASSERT_TRUE(theProvince.isColony());
}

TEST(EU4World_ProvinceTests, coresCanBeSetWithNewStyle)
{
	std::stringstream input;
	input << "cores={\n";
	input << "	\"TAG\"\n";
	input << "}\n";
	const EU4::Province theProvince("-1", input);

	ASSERT_TRUE(theProvince.getCores().count("TAG"));
}

TEST(EU4World_ProvinceTests, coresCanBeSetWithOldStyle)
{
	std::stringstream input;
	input << "core=\"TAG\"\n";
	const EU4::Province theProvince("-1", input);

	ASSERT_TRUE(theProvince.getCores().count("TAG"));
}

TEST(EU4World_ProvinceTests, coresCanBeAdded)
{
	std::stringstream input;
	EU4::Province theProvince("-1", input);
	theProvince.addCore("TAG");

	ASSERT_TRUE(theProvince.getCores().count("TAG"));
}

TEST(EU4World_ProvinceTests, coresCanBeRemoved)
{
	std::stringstream input;
	input << "core=\"TAG\"\n";
	EU4::Province theProvince("-1", input);
	theProvince.removeCore("TAG");

	ASSERT_FALSE(theProvince.getCores().count("TAG"));
}

TEST(EU4World_ProvinceTests, inHreCanRemainNegative)
{
	std::stringstream input;
	input << "hre=no\n";
	const EU4::Province theProvince("-1", input);

	ASSERT_FALSE(theProvince.inHre());
}

TEST(EU4World_ProvinceTests, inHreCanBeTrue)
{
	std::stringstream input;
	input << "hre=yes\n";
	const EU4::Province theProvince("-1", input);

	ASSERT_TRUE(theProvince.inHre());
}

TEST(EU4World_ProvinceTests, wasColonizedDefaultsToFalse)
{
	std::stringstream input;
	const EU4::Province theProvince("-1", input);

	ASSERT_FALSE(theProvince.wasColonized());
}

TEST(EU4World_ProvinceTests, wasColonizedTrueIfHasOriginalColoniser)
{
	std::stringstream input;
	input << "original_coloniser=\"TAG\"\n";
	const EU4::Province theProvince("-1", input);

	ASSERT_TRUE(theProvince.wasColonized());
}

TEST(EU4World_ProvinceTests, wasColonizedFalseIfOwnedAtStart)
{
	std::stringstream input;
	input << "owner=\"TAG\"\n";
	const EU4::Province theProvince("-1", input);

	ASSERT_FALSE(theProvince.wasColonized());
}

TEST(EU4World_ProvinceTests, wasColonizedFalseIfHasOriginalCulture)
{
	std::stringstream input;
	input << "culture=theCulture\n";
	input << "history={\n";
	input << "\t1600.1.1={\n";
	input << "\t\towner=\"TAG\"\n";
	input << "\t}\n";
	input << "}\n";
	const EU4::Province theProvince("-1", input);

	ASSERT_FALSE(theProvince.wasColonized());
}

TEST(EU4World_ProvinceTests, wasColonizedTrueIfCultureInHistoryChanged)
{
	std::stringstream input;
	input << "history={\n";
	input << "culture=theCulture\n";
	input << "\t1600.1.1={\n";
	input << "\t\towner=\"TAG\"\n";
	input << "\t\tculture=newCulture\n";
	input << "\t}\n";
	input << "}\n";
	const EU4::Province theProvince("-1", input);

	ASSERT_TRUE(theProvince.wasColonized());
}

TEST(EU4World_ProvinceTests, getFirstOwnedDateCanDetectOwnershipAtStartDate)
{
	std::stringstream input;
	input << "history={\n";
	input << "\towner=\"TAG\"\n";
	input << "}\n";
	const EU4::Province theProvince("-1", input);

	ASSERT_EQ(date("1000.1.1"), *theProvince.getFirstOwnedDate());
}

TEST(EU4World_ProvinceTests, getFirstOwnedDateCanDetectLaterOwnership)
{
	std::stringstream input;
	input << "history={\n";
	input << "\t1600.1.1={\n";
	input << "\t\towner=\"TAG\"\n";
	input << "\t}\n";
	input << "}\n";
	const EU4::Province theProvince("-1", input);

	ASSERT_EQ(date("1600.1.1"), theProvince.getFirstOwnedDate());
}

TEST(EU4World_ProvinceTests, hasBuildingDefaultsToFalse)
{
	std::stringstream input;
	const EU4::Province theProvince("-1", input);

	ASSERT_FALSE(theProvince.hasBuilding("theBuilding"));
}

TEST(EU4World_ProvinceTests, hasBuildingCanBeTrue)
{
	std::stringstream input;
	input << "buildings={\n";
	input << "\ttheBuilding=yes\n";
	input << "}\n";
	const EU4::Province theProvince("-1", input);

	ASSERT_TRUE(theProvince.hasBuilding("theBuilding"));
}

TEST(EU4World_ProvinceTests, hasGreatProjectDefaultsToFalse)
{
	std::stringstream input;
	const EU4::Province theProvince("-1", input);

	ASSERT_FALSE(theProvince.hasGreatProject("test_canal"));
}

TEST(EU4World_ProvinceTests, hasGreatProjectFindsProjects)
{
	std::stringstream input;
	input << "great_projects={\n";
	input << "\t\"test_canal\"\n";
	input << "}\n";
	const EU4::Province theProvince("-1", input);

	ASSERT_TRUE(theProvince.hasGreatProject("test_canal"));
}

TEST(EU4World_ProvinceTests, hasModifierDefaultsToFalse)
{
	std::stringstream input;
	const EU4::Province theProvince("-1", input);

	ASSERT_FALSE(theProvince.hasModifier("center_of_trade_modifier"));
}

TEST(EU4World_ProvinceTests, hasModifierFindsModifiers)
{
	std::stringstream input;
	input << "modifier={\n";
	input << "\tmodifier=\"center_of_trade_modifier\"\n";
	input << "}\n";
	const EU4::Province theProvince("-1", input);

	ASSERT_TRUE(theProvince.hasModifier("center_of_trade_modifier"));
}
