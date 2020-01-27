#include "Modifier.h"
#include "ParserHelpers.h"
#include "Log.h"

EU4::Modifier::Modifier(std::istream& theStream)
{
	registerKeyword("potential", commonItems::ignoreItem);
	registerKeyword("trigger", commonItems::ignoreItem);

	registerRegex("[a-zA-Z0-9_]+", [this](const std::string& effect, std::istream& theStream) {
		const commonItems::singleString amountSingleString(theStream);
		const auto amountString = amountSingleString.getString();

		std::smatch match;
		if (std::regex_match(amountString, match, std::regex("-?[0-9]+([.][0-9]+)?")))
		{
			double amount = stof(amountString);
			effects.insert(std::make_pair(effect, amount));
		}
	});

	parseStream(theStream);
	clearRegisteredKeywords();
}

double EU4::Modifier::getEffectAmount(const std::string& modifier) const
{
	if (effects.count(modifier) > 0) return effects.at(modifier);
	return 0;
}