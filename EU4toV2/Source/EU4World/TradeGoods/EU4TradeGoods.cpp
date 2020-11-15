#include "EU4TradeGoods.h"
#include "EU4TradeGood.h"
#include "ParserHelpers.h"

EU4::TradeGoods::TradeGoods(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU4::TradeGoods::registerKeys()
{
	registerRegex("[a-z_]+", [this](const std::string& tradeGood, std::istream& theStream) {
		const TradeGood newGood(theStream);
		tradeGoods.insert(std::make_pair(tradeGood, newGood.getPrice()));
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

std::optional<double> EU4::TradeGoods::getPrice(const std::string& good) const
{
	if (const auto& goodItr = tradeGoods.find(good); goodItr != tradeGoods.end())
		return goodItr->second;
	if (const auto& unknownItr = tradeGoods.find("unknown"); unknownItr != tradeGoods.end())
		return unknownItr->second;
	return std::nullopt;
}
