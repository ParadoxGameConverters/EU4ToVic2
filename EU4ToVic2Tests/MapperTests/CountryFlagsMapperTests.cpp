#include "gtest/gtest.h"
#include "../../EU4toV2/Source/Mappers/CountryFlags/CountryFlagsMapping.h"

TEST(Mappers_CountryFlagsMappingTests, SourceFlagDefaultsToBlank)
{
	std::stringstream input;
	const mappers::CountryFlagsMapping countryFlagsMapping(input);

	ASSERT_TRUE(countryFlagsMapping.getEU4Flag().empty());
}

TEST(Mappers_CountryFlagsMappingTests, SourceFlagCanBeSet)
{
	std::stringstream input;
	input << "eu4 = changed_from_colonial_nation";
	mappers::CountryFlagsMapping countryFlagsMapping(input);

	ASSERT_EQ(countryFlagsMapping.getEU4Flag(), "changed_from_colonial_nation");
}

TEST(Mappers_CountryFlagsMappingTests, TargetFlagsDefaultToBlank)
{
	std::stringstream input;
	const mappers::CountryFlagsMapping countryFlagsMapping(input);

	ASSERT_TRUE(countryFlagsMapping.getV2Flags().empty());
}

TEST(Mappers_CountryFlagsMappingTests, TargetFlagsCanBeSet)
{
	std::stringstream input;
	input << "v2 = post_colonial_country v2 = postcol_1";
	mappers::CountryFlagsMapping countryFlagsMapping(input);

	ASSERT_EQ(countryFlagsMapping.getV2Flags()[0], "post_colonial_country");
	ASSERT_EQ(countryFlagsMapping.getV2Flags()[1], "postcol_1");
}

