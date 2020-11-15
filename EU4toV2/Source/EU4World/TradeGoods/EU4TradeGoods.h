#ifndef EU4TRADE_GOODS_H
#define EU4TRADE_GOODS_H
#include "Parser.h"

namespace EU4
{
class TradeGoods: commonItems::parser
{
  public:
	TradeGoods() = default;
	explicit TradeGoods(std::istream& theStream);

	[[nodiscard]] std::optional<double> getPrice(const std::string& good) const;

  private:
	void registerKeys();

	std::map<std::string, double> tradeGoods;
};
} // namespace EU4

#endif // EU4TRADE_GOODS_H