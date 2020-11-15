#include "TradeGoods/EU4TradeGoods.h"
#include "gtest/gtest.h"

TEST(EU4World_TradeGoodsTests, pricesCanBeMatched)
{
	std::stringstream input;
	input << "bananas = {\n";
	input << "\tcurrent_price = 1.5\n";
	input << "}\n";
	input << "cement = {\n";
	input << "\tcurrent_price = 2.5\n";
	input << "}\n";
	input << "unknown = {\n";
	input << "\tcurrent_price = 0\n";
	input << "}\n";
	const EU4::TradeGoods goods(input);

	ASSERT_EQ(1.5, goods.getPrice("bananas"));
	ASSERT_EQ(2.5, goods.getPrice("cement"));
}

TEST(EU4World_TradeGoodsTests, priceReturnsUnknownForMismatch)
{
	std::stringstream input;
	input << "unknown = {\n";
	input << "\tcurrent_price = 1\n";
	input << "}\n";
	const EU4::TradeGoods goods(input);

	ASSERT_EQ(1.0, goods.getPrice("bananas"));
}

TEST(EU4World_TradeGoodsTests, priceReturnsNulloptForNoUnknown)
{
	std::stringstream input;
	const EU4::TradeGoods goods(input);

	ASSERT_EQ(std::nullopt, goods.getPrice("bananas"));
}
