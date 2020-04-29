#ifndef EU4TRADE_GOOD_H
#define EU4TRADE_GOOD_H

#include "Date.h"
#include "Parser.h"

namespace EU4
{
	class TradeGood :  commonItems::parser
	{
	public:
		TradeGood() = default;
		explicit TradeGood(std::istream& theStream);
		
		[[nodiscard]] double getPrice() const { return price; }

	private:
		double price = 0;
	};
}

#endif // EU4TRADE_GOOD_H