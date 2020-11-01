#include "gtest/gtest.h"
#include "UnitTypes/UnitType.h"

TEST(EU4World_EU4RegimentTests, regimentInitsDefaults)
{
	std::stringstream input;
	EU4::EU4Regiment regiment(input);

	ASSERT_TRUE(regiment.getType().empty());
	ASSERT_TRUE(regiment.getName().empty());
	ASSERT_EQ(0, regiment.getHome());
	ASSERT_EQ(0, regiment.getTypeStrength());
	ASSERT_EQ(0.0, regiment.getStrength());
	ASSERT_EQ(EU4::REGIMENTCATEGORY::num_reg_categories, regiment.getCategory());
}

TEST(EU4World_EU4RegimentTests, regimentLoadsPrimitives)
{
	std::stringstream input;
	input << "type = cavalry\n";
	input << "unit_type = sub_saharan\n";
	input << "maneuver = 2\n";
	input << "offensive_morale = 3\n";
	input << "defensive_morale = 3\n";
	input << "offensive_fire = 0\n";
	input << "defensive_fire = 0\n";
	input << "offensive_shock = 2\n";
	input << "defensive_shock = 2\n";
	
	EU4::EU4Regiment regiment(input);

	ASSERT_EQ("cavalry", regiment.getType());
	ASSERT_TRUE(regiment.getName().empty());
	ASSERT_EQ(0, regiment.getHome());
	ASSERT_EQ(0, regiment.getTypeStrength());
	ASSERT_EQ(0.0, regiment.getStrength());
	ASSERT_EQ(EU4::REGIMENTCATEGORY::num_reg_categories, regiment.getCategory());
}
