#include "VNColonialMapper/VNColonialMapping.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>
using testing::ElementsAre;

TEST(Mappers_VNColonialMappingTests, MappingDefaultsToDefaults)
{
	std::stringstream input;
	const mappers::VNColonialMapping theMapping(input);

	EXPECT_TRUE(theMapping.getName().empty());
	EXPECT_EQ(0, theMapping.getKeyProvince());
	EXPECT_TRUE(theMapping.getProvinces().empty());
}

TEST(Mappers_VNColonialMappingTests, MappingCanBeLoaded)
{
	std::stringstream input;
	input << R"(name = "British Pacifics" key = 290 provinces = { 2524 2508 })";
	const mappers::VNColonialMapping theMapping(input);

	EXPECT_EQ("British Pacifics", theMapping.getName());
	EXPECT_EQ(290, theMapping.getKeyProvince());
	EXPECT_THAT(theMapping.getProvinces(), ElementsAre(2524, 2508));
}
