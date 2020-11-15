#include "TradeGoods/EU4TradeGood.h"
#include "gtest/gtest.h"

TEST(EU4World_TradeGoodTests, priceDefaultsToZero)
{
	std::stringstream input;
	const EU4::TradeGood good(input);

	ASSERT_EQ(0, good.getPrice());
}

TEST(EU4World_TradeGoodTests, priceCanBeSet)
{
	std::stringstream input;
	input << "current_price = 3.5";
	const EU4::TradeGood good(input);

	ASSERT_EQ(3.5, good.getPrice());
}
