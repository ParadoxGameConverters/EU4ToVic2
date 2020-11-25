#include "CulturalUnions/CulturalUnionMapper.h"
#include "gtest/gtest.h"

TEST(Mappers_CulturalUnionMapperTests, nonMatchReturnsNullopt)
{
	std::stringstream input;
	const mappers::CulturalUnionMapper culturalUnionMapper(input);

	ASSERT_EQ(std::nullopt, culturalUnionMapper.getCoresForCulture("a_culture"));
}

TEST(Mappers_CulturalUnionMapperTests, matchReturnsCoreTags)
{
	std::stringstream input("link = { tag = TAG tag = TA2 culture = a_culture }");
	const mappers::CulturalUnionMapper theCulturalUnionMapper(input);

	ASSERT_EQ(2, theCulturalUnionMapper.getCoresForCulture("a_culture")->size());
	ASSERT_EQ("TAG", (*theCulturalUnionMapper.getCoresForCulture("a_culture"))[0]);
	ASSERT_EQ("TA2", (*theCulturalUnionMapper.getCoresForCulture("a_culture"))[1]);
}
