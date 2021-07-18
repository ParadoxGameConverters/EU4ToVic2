#include "../EU4toV2/Source/Mappers/ReligionMapper/ReligionMapping.h"
#include "gtest/gtest.h"
#include <sstream>



TEST(Mappers_ReligionMappingTests, vic2ReligionDefaultsToBlank)
{
	std::stringstream input;
	input << "= {}";

	mappers::ReligionMapping theMapping(input);

	ASSERT_EQ(theMapping.getVic2Religion(), "");
}


TEST(Mappers_ReligionMappingTests, vic2ReligionCanBeSet)
{
	std::stringstream input;
	input << "= { v2 = vic2Religion }";

	mappers::ReligionMapping theMapping(input);

	ASSERT_EQ(theMapping.getVic2Religion(), "vic2Religion");
}


TEST(Mappers_ReligionMappingTests, eu4ReligionsDefaultToEmpty)
{
	std::stringstream input;
	input << "= {}";

	mappers::ReligionMapping theMapping(input);

	ASSERT_EQ(theMapping.getEU4Religions().size(), 0);
}


TEST(Mappers_ReligionMappingTests, eu4ReligionsCanBeSet)
{
	std::stringstream input;
	input << "= { eu4 = religion1 eu4 = religion2 }";

	mappers::ReligionMapping theMapping(input);

	ASSERT_EQ(theMapping.getEU4Religions().size(), 2);
	ASSERT_EQ(theMapping.getEU4Religions()[0], "religion1");
	ASSERT_EQ(theMapping.getEU4Religions()[1], "religion2");
}