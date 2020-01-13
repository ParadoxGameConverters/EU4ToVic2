#include "gtest/gtest.h"
#include "../EU4toV2/Source/Mappers/CulturalUnions/CulturalUnion.h"


TEST(Mappers_CulturalUnionTests, defaultCultureIsBlank)
{
	std::stringstream input("");
	mappers::CulturalUnion theCultureUnion(input);

	ASSERT_EQ(theCultureUnion.getUnion().first, "");
}


TEST(Mappers_CulturalUnionTests, cultureIsImported)
{
	std::stringstream input("culture = a_culture");
	mappers::CulturalUnion theCultureUnion(input);

	ASSERT_EQ(theCultureUnion.getUnion().first, "a_culture");
}


TEST(Mappers_CulturalUnionTests, defaultNoTags)
{
	std::stringstream input("");
	mappers::CulturalUnion theCultureUnion(input);

	ASSERT_EQ(theCultureUnion.getUnion().second.size(), 0);
}


TEST(Mappers_CulturalUnionTests, tagsAreImported)
{
	std::stringstream input("tag = TAG tag = TG2");
	mappers::CulturalUnion theCultureUnion(input);

	auto theUnion = theCultureUnion.getUnion();
	ASSERT_EQ(theUnion.second[0], "TAG");
	ASSERT_EQ(theUnion.second[1], "TG2");
}