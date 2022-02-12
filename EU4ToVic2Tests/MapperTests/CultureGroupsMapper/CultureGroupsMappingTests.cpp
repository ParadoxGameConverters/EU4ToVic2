#include "CultureGroupsMapper/CultureGroupsMapping.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>
#include "Configuration.h"
using testing::ElementsAre;

TEST(Mappers_CultureGroupsMappingTests, defaultsDefaultToDefaults)
{
	std::stringstream input;
	const mappers::CultureGroupsMapping mapping(input);

	EXPECT_TRUE(mapping.getEU4s().empty());
	EXPECT_TRUE(mapping.getV2().empty());
}

TEST(Mappers_CultureGroupsMappingTests, mappingCanBeLoaded)
{
	std::stringstream input;
	input << "v2 = v2group eu4 = eu4group1 eu4 = eu4group2";
	const mappers::CultureGroupsMapping mapping(input);

	EXPECT_THAT(mapping.getEU4s(), ElementsAre("eu4group1", "eu4group2"));
	EXPECT_EQ("v2group", mapping.getV2());
}

TEST(Mappers_CultureGroupsMappingTests, HPMDoesNotOverrideIfDisabled)
{
	std::stringstream input;
	input << "v2 = v2group eu4 = eu4group1 eu4 = eu4group2 hpm = HPMgroup";
	const mappers::CultureGroupsMapping mapping(input);

	EXPECT_THAT(mapping.getEU4s(), ElementsAre("eu4group1", "eu4group2"));
	EXPECT_EQ("v2group", mapping.getV2());
}

TEST(Mappers_CultureGroupsMappingTests, HPMOVerridesIfEenabled)
{
	theConfiguration.setHybridMod(Configuration::HYBRIDMOD::HPM);
	std::stringstream input;
	input << "v2 = v2group eu4 = eu4group1 eu4 = eu4group2 hpm = HPMgroup";
	const mappers::CultureGroupsMapping mapping(input);

	EXPECT_THAT(mapping.getEU4s(), ElementsAre("eu4group1", "eu4group2"));
	EXPECT_EQ("HPMgroup", mapping.getV2());
}
