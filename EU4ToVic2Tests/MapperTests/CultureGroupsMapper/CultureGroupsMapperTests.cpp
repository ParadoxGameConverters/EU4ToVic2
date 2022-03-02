#include "CultureGroupsMapper/CultureGroupsMapper.h"
#include "gtest/gtest.h"

TEST(Mappers_CultureGroupsMapperTests, CultureGroupCanBePinged)
{
	std::stringstream input;
	input << "link = { v2 = v2group1 eu4 = eu4group1 } \n";
	input << "link = { v2 = v2group2 eu4 = eu4group2 } \n";
	const mappers::CultureGroupsMapper mapper(input);

	EXPECT_EQ("v2group1", mapper.getV2CultureGroup("eu4group1"));
	EXPECT_EQ("v2group2", mapper.getV2CultureGroup("eu4group2"));
}

TEST(Mappers_CultureGroupsMapperTests, MapperReturnsNulloptForMismatch)
{
	std::stringstream input;
	const mappers::CultureGroupsMapper mapper(input);

	EXPECT_EQ(std::nullopt, mapper.getV2CultureGroup("eu4group1"));
}