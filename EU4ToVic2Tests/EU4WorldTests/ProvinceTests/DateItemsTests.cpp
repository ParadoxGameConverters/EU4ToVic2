#include "Provinces/DateItems.h"
#include "gtest/gtest.h"

TEST(EU4World_Province_DateItemsTests, emptyItemsAreNotImported)
{
	std::stringstream input;
	const EU4::DateItems dateItems(input);

	ASSERT_TRUE(dateItems.getDateChanges().empty());
}

TEST(EU4World_Province_DateItemsTests, itemsCanBeImported)
{
	std::stringstream input;
	input << "owner = SWE\n";
	input << "religion = catholic\n";
	input << "culture = swedish\n";

	const EU4::DateItems dateItems(input);

	ASSERT_EQ(3, dateItems.getDateChanges().size());
	ASSERT_EQ("owner", dateItems.getDateChanges()[0].first);
	ASSERT_EQ("SWE", dateItems.getDateChanges()[0].second);
	ASSERT_EQ("religion", dateItems.getDateChanges()[1].first);
	ASSERT_EQ("catholic", dateItems.getDateChanges()[1].second);
	ASSERT_EQ("culture", dateItems.getDateChanges()[2].first);
	ASSERT_EQ("swedish", dateItems.getDateChanges()[2].second);
}
