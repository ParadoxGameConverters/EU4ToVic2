#include "gtest/gtest.h"
#include "../../EU4toV2/Source/Mappers/CountryFlags/CountryFlags.h"
#include "../../EU4toV2/Source/Mappers/CountryFlags/CountryFlagsMapping.h"
#include "../../EU4toV2/Source/V2World/Country/Country.h"

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
	const mappers::CountryFlagsMapping countryFlagsMapping(input);

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
	const mappers::CountryFlagsMapping countryFlagsMapping(input);

	ASSERT_EQ(countryFlagsMapping.getV2Flags()[0], "post_colonial_country");
	ASSERT_EQ(countryFlagsMapping.getV2Flags()[1], "postcol_1");
}

TEST(Mappers_CountryFlagsMappingTests, VerifyCountryFlags)
{
	std::stringstream input;
	input << "link = { eu4 = changed_from_colonial_nation v2 = post_colonial_country v2 = postcol_1 }";
	input << "link = { eu4 = eu4Flag v2 = v2Flag1 v2 = v2Flag2 }";
	input << "link = { eu4 = changed_from_colonial_nation v2 = post_colonial_country v2 = postcol_2 }";
	mappers::CountryFlags countryFlags(input);

	auto flagRangeItr = countryFlags.getFlags().equal_range("changed_from_colonial_nation");
	std::vector<std::string> v2Flags;
	while (flagRangeItr.first != flagRangeItr.second)
	{
		v2Flags.push_back(flagRangeItr.first->second);
		++flagRangeItr.first;
	}

	ASSERT_TRUE(std::find(v2Flags.begin(), v2Flags.end(), "post_colonial_country") != v2Flags.end());
	ASSERT_TRUE(std::find(v2Flags.begin(), v2Flags.end(), "postcol_1") != v2Flags.end());
	ASSERT_TRUE(std::find(v2Flags.begin(), v2Flags.end(), "postcol_2") != v2Flags.end());
	ASSERT_FALSE(std::find(v2Flags.begin(), v2Flags.end(), "v2Flag1") != v2Flags.end());
	ASSERT_FALSE(std::find(v2Flags.begin(), v2Flags.end(), "v2Flag2") != v2Flags.end());
}

