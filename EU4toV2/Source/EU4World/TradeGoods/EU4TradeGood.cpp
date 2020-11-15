#include "EU4TradeGood.h"
#include "ParserHelpers.h"

EU4::TradeGood::TradeGood(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU4::TradeGood::registerKeys()
{
	registerKeyword("current_price", [this](const std::string& unused, std::istream& theStream) {
		price = commonItems::singleDouble(theStream).getDouble();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
