#include "EU4TradeGoods.h"
#include "ParserHelpers.h"
#include "EU4TradeGood.h"

EU4::TradeGoods::TradeGoods(std::istream& theStream)
{
	registerRegex("[a-z_]+", [this](const std::string& tradeGood, std::istream& theStream)
		{
			TradeGood newGood(theStream);
			tradeGoods.insert(std::make_pair(tradeGood, newGood.getPrice()));
		});

	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}

std::optional<double> EU4::TradeGoods::getPrice(const std::string& good) const
{
	const auto& goodItr = tradeGoods.find(good);
	if (goodItr != tradeGoods.end()) return goodItr->second;
	const auto& unknownItr = tradeGoods.find("unknown");
	if (unknownItr != tradeGoods.end()) return unknownItr->second;
	return std::nullopt;
}