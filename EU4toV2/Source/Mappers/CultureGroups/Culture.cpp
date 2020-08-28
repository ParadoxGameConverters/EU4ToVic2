#include "Culture.h"
#include "ParserHelpers.h"
#include <random>

mappers::Culture::Culture(std::istream& theStream)
{
	registerKeyword("primary", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString primaryStr(theStream);
		primaryTag = primaryStr.getString();
	});
	registerKeyword("graphical_culture", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString graphicStr(theStream);
		graphicalCulture = graphicStr.getString();
	});
	registerKeyword("male_names", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::stringList names(theStream);
		maleNames = names.getStrings();
	});
	registerKeyword("female_names", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::stringList names(theStream);
		femaleNames = names.getStrings();
	});
	registerKeyword("first_names", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::stringList names(theStream);
		firstNames = names.getStrings();
	});
	registerKeyword("last_names", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::stringList names(theStream);
		lastNames = names.getStrings();
	});
	registerKeyword("dynasty_names", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::stringList names(theStream);
		dynastyNames = names.getStrings();
	});
	registerKeyword("color", [this](const std::string& unused, std::istream& theStream) {
		color = commonItems::Color::Factory::getColor(theStream);
	});
	registerKeyword("radicalism", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt radicalInt(theStream);
		radicalism = radicalInt.getInt();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::Culture::transmogrify()
{
	firstNames = maleNames;
	lastNames = dynastyNames;

	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> distr(0, 255);

	const auto r = distr(eng);
	const auto g = distr(eng);
	const auto b = distr(eng);
	color = commonItems::Color(std::array<int, 3>{r, g, b});
	primaryTag.clear();
}
