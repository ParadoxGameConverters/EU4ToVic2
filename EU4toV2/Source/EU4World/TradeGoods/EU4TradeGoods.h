#ifndef EU4TRADE_GOODS_H
#define EU4TRADE_GOODS_H

#include "Date.h"
#include "newParser.h"

namespace EU4
{
	class TradeGoods : commonItems::parser
	{
	public:
		TradeGoods() = default;
		explicit TradeGoods(std::istream& theStream);
		[[nodiscard]] std::optional<double> getPrice(const std::string& good) const;

	private:
		std::map<std::string, double> tradeGoods;
	};
}

#endif // EU4TRADE_GOODS_H