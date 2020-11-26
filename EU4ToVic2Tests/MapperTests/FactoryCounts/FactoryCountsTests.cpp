#include "FactoryCounts/FactoryStartingCounts.h"
#include "gtest/gtest.h"

TEST(Mappers_FactoryCountsTests, startingFactoriesEmpty)
{
	std::stringstream input;
	const mappers::FactoryStartingCounts mapper(input);

	ASSERT_TRUE(mapper.getFactoryStartingCounts().empty());
}

TEST(Mappers_FactoryCountsTests, countsCanBeLoaded)
{
	std::stringstream input;
	input << "factory1 = 3\n";
	input << "factory2 = 5\n";
	const mappers::FactoryStartingCounts mapper(input);

	ASSERT_EQ(3, mapper.getFactoryStartingCounts().at("factory1"));
	ASSERT_EQ(5, mapper.getFactoryStartingCounts().at("factory2"));
}
