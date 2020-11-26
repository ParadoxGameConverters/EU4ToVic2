#include "CulturalUnions/CulturalUnionMapper.h"
#include "gtest/gtest.h"

TEST(Mappers_CulturalUnionMapperTests, nonMatchReturnsEmpty)
{
	std::stringstream input;
	const mappers::CulturalUnionMapper culturalUnionMapper(input);

	ASSERT_TRUE(culturalUnionMapper.getCoresForCulture("a_culture").empty());
}

TEST(Mappers_CulturalUnionMapperTests, matchReturnsCoreTags)
{
	std::stringstream input("link = { tag = TAG tag = TA2 culture = a_culture }");
	const mappers::CulturalUnionMapper theCulturalUnionMapper(input);

	ASSERT_EQ(2, theCulturalUnionMapper.getCoresForCulture("a_culture").size());
	ASSERT_EQ("TAG", theCulturalUnionMapper.getCoresForCulture("a_culture")[0]);
	ASSERT_EQ("TA2", theCulturalUnionMapper.getCoresForCulture("a_culture")[1]);
}
