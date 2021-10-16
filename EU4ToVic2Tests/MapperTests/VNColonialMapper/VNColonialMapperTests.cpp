#include "VNColonialMapper/VNColonialMapper.h"
#include "gtest/gtest.h"

TEST(Mappers_VNColonialMapperTests, ColoniesDefaultToEmpty)
{
	std::stringstream input;
	const mappers::VNColonialMapper mapper(input);

	EXPECT_TRUE(mapper.getVNColonies().empty());
}

TEST(Mappers_VNColonialMapperTests, ColoniesCanBeLoaded)
{
	std::stringstream input;
	input << R"(link = { name = "British Pacifics" key = 290 provinces = { 2524 2508 } })";
	input << R"(link = { name = "British SEA" key = 300 provinces = { 1320 1343 1344 1345 1384 1386 1388 } })";
	const mappers::VNColonialMapper mapper(input);

	EXPECT_EQ(2, mapper.getVNColonies().size());
	EXPECT_EQ("British Pacifics", mapper.getVNColonies()[0].getName());
	EXPECT_EQ("British SEA", mapper.getVNColonies()[1].getName());
}
