#include "Modifier.h"
#include "ParserHelpers.h"

EU4::Modifier::Modifier(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU4::Modifier::registerKeys()
{
	registerKeyword("potential", commonItems::ignoreItem);
	registerKeyword("trigger", commonItems::ignoreItem);
	registerRegex(commonItems::catchallRegex, [this](const std::string& effect, std::istream& theStream) {
		const auto amountString = commonItems::singleString(theStream).getString();

		std::smatch match;
		if (std::regex_match(amountString, match, std::regex("-?[0-9]+([.][0-9]+)?")))
		{
			auto amount = std::stod(amountString);
			effects.insert(std::make_pair(effect, amount));
		}
	});
}

double EU4::Modifier::getEffectAmount(const std::string& modifier) const
{
	if (effects.count(modifier))
		return effects.at(modifier);
	return 0;
}