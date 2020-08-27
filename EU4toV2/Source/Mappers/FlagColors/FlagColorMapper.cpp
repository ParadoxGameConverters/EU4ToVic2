#include "FlagColorMapper.h"
#include "../../Configuration.h"
#include "Log.h"
#include "ParserHelpers.h"

mappers::FlagColorMapper::FlagColorMapper()
{
	LOG(LogLevel::Info) << "Parsing EU4 custom flag colours.";
	registerKeys();
	parseFile(theConfiguration.getEU4Path() + "/common/custom_country_colors/00_custom_country_colors.txt");
	clearRegisteredKeywords();
}

mappers::FlagColorMapper::FlagColorMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::FlagColorMapper::registerKeys()
{
	registerKeyword("flag_color", [this](const std::string& unused, std::istream& theStream) {
		const auto theColor = commonItems::newColor::Factory::getColor(theStream);
		flagColorMapping.push_back(theColor);
	});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}

std::optional<commonItems::newColor> mappers::FlagColorMapper::getFlagColorByIndex(int index) const
{
	if (flagColorMapping.empty() || static_cast<long>(index) >= static_cast<long>(flagColorMapping.size()))
		return std::nullopt;
	return flagColorMapping[index];
}
