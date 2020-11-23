#include "ColonialTags/ColonialTag.h"
#include "gtest/gtest.h"

TEST(Mappers_ColonialTagTests, primitivesDefaultToDefaults)
{
	std::stringstream input;
	const mappers::ColonialTag tag(input);

	ASSERT_TRUE(tag.getColonyTag().tag.empty());
	ASSERT_TRUE(tag.getColonyTag().EU4Regions.empty());
	ASSERT_TRUE(tag.getColonyTag().V2Regions.empty());
	ASSERT_TRUE(tag.getColonyTag().cultureGroups.empty());
}

TEST(Mappers_ColonialTagTests, primitivesCanBeLoaded)
{
	std::stringstream input;
	input << "tag = TAG eu4region = e4r1 eu4region = e4r2 v2region = v2r1 v2region = v2r2 cultureGroup = cg1 cultureGroup = cg2\n";
	const mappers::ColonialTag tag(input);

	ASSERT_EQ("TAG", tag.getColonyTag().tag);
	ASSERT_TRUE(tag.getColonyTag().EU4Regions.contains("e4r1"));
	ASSERT_TRUE(tag.getColonyTag().EU4Regions.contains("e4r2"));
	ASSERT_TRUE(tag.getColonyTag().V2Regions.contains("v2r1"));
	ASSERT_TRUE(tag.getColonyTag().V2Regions.contains("v2r2"));
	ASSERT_TRUE(tag.getColonyTag().cultureGroups.contains("cg1"));
	ASSERT_TRUE(tag.getColonyTag().cultureGroups.contains("cg2"));
}
