#include "CulturalUnions/CulturalUnion.h"
#include "gtest/gtest.h"

TEST(Mappers_CulturalUnionTests, primitivesDefaultToDefaults)
{
	std::stringstream input;
	const mappers::CulturalUnion theCultureUnion(input);

	ASSERT_TRUE(theCultureUnion.getCulture().empty());
	ASSERT_TRUE(theCultureUnion.getTags().empty());
}

TEST(Mappers_CulturalUnionTests, primitivesCanBeLoaded)
{
	std::stringstream input;
	input << "culture = a_culture tag = TAG tag = TG2";
	const mappers::CulturalUnion theCultureUnion(input);

	ASSERT_EQ("a_culture", theCultureUnion.getCulture());
	ASSERT_EQ("TAG", theCultureUnion.getTags()[0]);
	ASSERT_EQ("TG2", theCultureUnion.getTags()[1]);
}
