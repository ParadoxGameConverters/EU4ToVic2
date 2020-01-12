#include "Modifier.h"
#include "ParserHelpers.h"
#include "Log.h"

EU4::Modifier::Modifier(std::istream& theStream)
{
	registerKeyword("potential", commonItems::ignoreItem);
	registerKeyword("trigger", commonItems::ignoreItem);

	registerRegex("[a-zA-Z0-9_]+", [this](const std::string& effect, std::istream& theStream) {
		commonItems::singleString amountSingleString(theStream);
		auto amountString = amountSingleString.getString();

		std::smatch match;
		if (std::regex_match(amountString, match, std::regex("-?[0-9]+([.][0-9]+)?")))
		{
			double amount = stof(amountString);
			effects.insert(std::make_pair(effect, amount));
		}
	});

	parseStream(theStream);
}

double EU4::Modifier::getEffectAmount(const std::string& theEffect) const
{
	if (effects.count(theEffect) > 0) return effects.at(theEffect);
	return 0;
}