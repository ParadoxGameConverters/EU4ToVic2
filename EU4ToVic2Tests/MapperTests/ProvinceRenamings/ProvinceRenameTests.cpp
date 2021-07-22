#include "ProvinceRenamings/ProvinceRename.h"
#include "gtest/gtest.h"

TEST(Mappers_ProvinceRenameTests, renamesDefaultToBlank)
{
	std::stringstream input;
	const mappers::ProvinceRename mapping(input);

	EXPECT_EQ(0, mapping.getProvinceID());
	EXPECT_TRUE(mapping.getCulture().empty());
	EXPECT_TRUE(mapping.getName().empty());
}

TEST(Mappers_ProvinceRenameTests, renameCanBeLoaded)
{
	std::stringstream input;
	input << R"(province = 100 culture = "theculture" name = "thename")";
	const mappers::ProvinceRename mapping(input);

	EXPECT_EQ(100, mapping.getProvinceID());
	EXPECT_EQ("theculture", mapping.getCulture());
	EXPECT_EQ("thename", mapping.getName());
}
