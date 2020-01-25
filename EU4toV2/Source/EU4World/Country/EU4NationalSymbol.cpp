#include "EU4NationalSymbol.h"
#include "ParserHelpers.h"

EU4::NationalSymbol::NationalSymbol(std::istream& theStream)
{
	registerKeyword("map_color", [this](const std::string& unused, std::istream& theStream)
		{
			mapColor = commonItems::Color(theStream);
		});
	registerKeyword("revolutionary_colors", [this](const std::string& unused, std::istream& theStream)
		{
			revolutionaryColor = commonItems::Color(theStream);
		});
	registerKeyword("country_color", [this](const std::string& unused, std::istream& theStream)
		{
			countryColor = commonItems::Color(theStream);
		});
	registerKeyword("custom_colors", [this](const std::string& colorsString, std::istream& theStream)
		{
			const CustomColors theSection(theStream);
			customColors = theSection;
			customColorsInitialized = true;
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
