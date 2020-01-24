#include "EU4TradeGood.h"
#include "ParserHelpers.h"

EU4::TradeGood::TradeGood(std::istream& theStream)
{
	registerKeyword("current_price", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleDouble priceDbl(theStream);
			price = priceDbl.getDouble();
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}


