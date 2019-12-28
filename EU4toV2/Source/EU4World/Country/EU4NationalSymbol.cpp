#include "EU4NationalSymbol.h"
#include "Log.h"
#include "NewParserToOldParserConverters.h"
#include "ParserHelpers.h"
#include <algorithm>


EU4::NationalSymbol::NationalSymbol(std::istream& theStream)
{
	registerKeyword(std::regex("map_color"), [this](const std::string& unused, std::istream& theStream)
		{
			mapColor = commonItems::Color(theStream);
		}
	);
	registerKeyword(std::regex("revolutionary_colors"), [this](const std::string& unused, std::istream& theStream)
		{
			revolutionaryColor = commonItems::Color(theStream);
		}
	);
	registerKeyword(std::regex("country_color"), [this](const std::string& unused, std::istream& theStream)
		{
			countryColor = commonItems::Color(theStream);
		}
	);
	registerKeyword(std::regex("custom_colors"), [this](const std::string& colorsString, std::istream& theStream)
		{
			EU4::CustomColors theSection(theStream);
			customColors = theSection;
			customColorsInitialized = true;
		}
	);

	registerKeyword(std::regex("[a-z0-9\\_]+"), commonItems::ignoreItem);
	parseStream(theStream);
}
