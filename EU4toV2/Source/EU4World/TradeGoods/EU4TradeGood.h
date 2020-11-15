#ifndef EU4TRADE_GOOD_H
#define EU4TRADE_GOOD_H
#include "Parser.h"

namespace EU4
{
class TradeGood: commonItems::parser
{
  public:
	TradeGood(std::istream& theStream);

	[[nodiscard]] double getPrice() const { return price; }

  private:
	void registerKeys();

	double price = 0;
};
} // namespace EU4

#endif // EU4TRADE_GOOD_H