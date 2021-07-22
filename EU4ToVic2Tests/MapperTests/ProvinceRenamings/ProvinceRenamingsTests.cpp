#include "ProvinceRenamings/ProvinceRenamings.h"
#include "gtest/gtest.h"

TEST(Mappers_ProvinceRenamingsTests, nonmatchProvinceReturnsEmptyOptional)
{
	std::stringstream input;
	input << R"(link = {province = 100 culture = "theculture" name = "thename"})";
	input << R"(link = {province = 100 culture = "theculture2" name = "thename2"})";
	input << R"(link = {province = 100 culture = "theculture" name = "thename3"})";
	input << R"(link = {province = 101 culture = "theculture" name = "thename4"})";
	const mappers::ProvinceRenamings mapper(input);

	EXPECT_EQ(std::nullopt, mapper.getRename(1, "theculture"));
}

TEST(Mappers_ProvinceRenamingsTests, nonmatchCultureReturnsEmptyOptional)
{
	std::stringstream input;
	input << R"(link = {province = 100 culture = "theculture" name = "thename"})";
	input << R"(link = {province = 100 culture = "theculture2" name = "thename2"})";
	input << R"(link = {province = 100 culture = "theculture" name = "thename3"})";
	input << R"(link = {province = 101 culture = "theculture" name = "thename4"})";
	const mappers::ProvinceRenamings mapper(input);

	EXPECT_EQ(std::nullopt, mapper.getRename(100, "nonsense"));
}

TEST(Mappers_ProvinceRenamingsTests, doubleMappedRenameReturnsLatestMapping)
{
	std::stringstream input;
	input << R"(link = {province = 100 culture = "theculture" name = "thename"})";
	input << R"(link = {province = 100 culture = "theculture2" name = "thename2"})";
	input << R"(link = {province = 100 culture = "theculture" name = "thename3"})";
	input << R"(link = {province = 101 culture = "theculture" name = "thename4"})";
	const mappers::ProvinceRenamings mapper(input);

	EXPECT_EQ("thename3", *mapper.getRename(100, "theculture"));
}

TEST(Mappers_ProvinceRenamingsTests, matchReturnsCorrectMapping)
{
	std::stringstream input;
	input << R"(link = {province = 100 culture = "theculture" name = "thename"})";
	input << R"(link = {province = 100 culture = "theculture2" name = "thename2"})";
	input << R"(link = {province = 100 culture = "theculture" name = "thename3"})";
	input << R"(link = {province = 101 culture = "theculture" name = "thename4"})";
	const mappers::ProvinceRenamings mapper(input);

	EXPECT_EQ("thename2", *mapper.getRename(100, "theculture2"));
	EXPECT_EQ("thename4", *mapper.getRename(101, "theculture"));
}
