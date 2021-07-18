#include "../EU4toV2/Source/Mappers/ReligionMapper/ReligionMapper.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(Mappers_ReligionMapperTests, nonMatchGivesEmptyOptional)
{
	std::stringstream input;
	input << "link = { v2 = vic2Religion eu4 = eu4Religion }";

	mappers::ReligionMapper theMapper(input);

	std::optional<std::string> vic2Religion = theMapper.getVic2Religion("nonMatchingReligion");
	ASSERT_FALSE(vic2Religion);
}


TEST(Mappers_ReligionMapperTests, vic2ReligionCanBeFound)
{
	std::stringstream input;
	input << "link = { v2 = vic2Religion eu4 = eu4Religion }";

	mappers::ReligionMapper theMapper(input);

	std::optional<std::string> vic2Religion = theMapper.getVic2Religion("eu4Religion");
	ASSERT_EQ(vic2Religion, "vic2Religion");
}


TEST(Mappers_ReligionMapperTests, multipleEU4ReligionsCanBeInARule)
{
	std::stringstream input;
	input << "link = { v2 = vic2Religion eu4 = eu4Religion eu4 = eu4Religion2 }";

	mappers::ReligionMapper theMapper(input);

	std::optional<std::string> vic2Religion = theMapper.getVic2Religion("eu4Religion2");
	ASSERT_EQ(vic2Religion, "vic2Religion");
}


TEST(Mappers_ReligionMapperTests, correctRuleMatches)
{
	std::stringstream input;
	input << "link = { v2 = vic2Religion eu4 = eu4Religion }";
	input << "link = { v2 = vic2Religion2 eu4 = eu4Religion2 }";

	mappers::ReligionMapper theMapper(input);

	std::optional<std::string> vic2Religion = theMapper.getVic2Religion("eu4Religion2");
	ASSERT_EQ(vic2Religion, "vic2Religion2");
}