#include "gtest/gtest.h"
#include "../EU4toV2/Source/EU4World/Religions/Religions.h"
#include <sstream>


TEST(EU4World_ReligionsTests, religionCanBeImported)
{
	std::stringstream input;
	input << "religion_group = {\n";
	input << "	religion = {\n";
	input << "	}\n";
	input << "}";
	EU4::Religions theReligions(input);

	ASSERT_TRUE(theReligions.getReligion("religion"));
}


TEST(EU4World_ReligionsTests, multipleReligionCanBeImported)
{
	std::stringstream input;
	input << "religion_group = {\n";
	input << "	religion = {\n";
	input << "	}\n";
	input << "	another_religion = {\n";
	input << "	}\n";
	input << "}";
	EU4::Religions theReligions(input);

	ASSERT_EQ(theReligions.getAllReligions().size(), 2);
}


TEST(EU4World_ReligionsTests, multipleReligionGroupsCanBeImported)
{
	std::stringstream input;
	input << "religion_group = {\n";
	input << "	religion = {\n";
	input << "	}\n";
	input << "}\n";
	input << "another_religion_group = {\n";
	input << "	another_religion = {\n";
	input << "	}\n";
	input << "}";
	EU4::Religions theReligions(input);

	ASSERT_EQ(theReligions.getAllReligions().size(), 2);
}
