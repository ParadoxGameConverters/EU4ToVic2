#include "ColonialRegions/ColonialRegions.h"
#include "gtest/gtest.h"

TEST(EU4World_ColonialRegionsTests, provincesCanBeMatched)
{
	std::stringstream input;
	input << "colonial_alaska = {\n";
	input << "\ttax_income = 2\n";
	input << "\tprovinces = {\n";
	input << "\t\t1 2 3\n";
	input << "\t}\n";
	input << "}\n";
	input << "colonial_placeholder = {\n";
	input << "\tcolor = { 200 190 20 }\n ";
	input << "}\n";

	const auto regions = EU4::ColonialRegions(input);

	ASSERT_TRUE(regions.provinceIsInColonialRegion(1, "colonial_alaska"));
}

TEST(EU4World_ColonialRegionsTests, provincesCanBeMisMatched)
{
	std::stringstream input;
	input << "colonial_alaska = {\n";
	input << "\ttax_income = 2\n";
	input << "\tprovinces = {\n";
	input << "\t\t1 2 3\n";
	input << "\t}\n";
	input << "}\n";
	input << "colonial_placeholder = {\n";
	input << "\tcolor = { 200 190 20 }\n ";
	input << "}\n";

	const auto regions = EU4::ColonialRegions(input);

	ASSERT_FALSE(regions.provinceIsInColonialRegion(1, "colonial_alabama"));
	ASSERT_FALSE(regions.provinceIsInColonialRegion(5, "colonial_alabama"));
	ASSERT_FALSE(regions.provinceIsInColonialRegion(5, "colonial_alaska"));
	ASSERT_FALSE(regions.provinceIsInColonialRegion(1, "colonial_placeholder"));
}
