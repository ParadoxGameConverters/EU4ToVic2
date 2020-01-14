#include "gtest/gtest.h"
#include "../EU4toV2/Source/mappers/BlockedTechSchools/BlockedTechSchools.h"



TEST(Mappers_BlockedSchoolsTests, techSchoolCanBeBlocked)
{
	std::stringstream input("blocked = a_blocked_tech_school");
	mappers::BlockedTechSchools theBlockedTechSchools(input);

	ASSERT_TRUE(theBlockedTechSchools.isTechSchoolBlocked("a_blocked_tech_school"));
}

TEST(Mappers_BlockedSchoolsTests, techSchoolCannotBeFound)
{
	std::stringstream input(
		"blocked = another_school\
		blocked = third_school");
	mappers::BlockedTechSchools theBlockedTechSchools(input);

	ASSERT_FALSE(theBlockedTechSchools.isTechSchoolBlocked("a_blocked_tech_school"));
}