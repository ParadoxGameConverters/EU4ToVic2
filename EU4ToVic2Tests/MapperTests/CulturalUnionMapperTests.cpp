#include "gtest/gtest.h"
#include "../EU4toV2/Source/Mappers/CulturalUnions/CulturalUnionMapper.h"


TEST(Mappers_CulturalUnionMapperTests, nonMatchGivesNoTags)
{
	std::stringstream input("");
	mappers::CulturalUnionMapper theCulturalUnionMapper(input);

	ASSERT_EQ(theCulturalUnionMapper.getCoresForCulture("a_culture").size(), 0);
}


TEST(Mappers_CulturalUnionMapperTests, matchGivesTag)
{
	std::stringstream input("link = { tag = TAG culture = a_culture }");
	mappers::CulturalUnionMapper theCulturalUnionMapper(input);

	ASSERT_EQ(theCulturalUnionMapper.getCoresForCulture("a_culture")[0], "TAG");
}