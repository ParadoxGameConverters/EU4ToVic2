#include "EU4CustomColors.h"
#include "ParserHelpers.h"

EU4::CustomColors::CustomColors(std::istream& theStream)
{
	registerKeyword("flag", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt flagInt(theStream);
			// Why, paradox, why?
			if (flagInt.getInt() < 0) 
			{
				customColors.flag = 1;
			}
			else
			{
				customColors.flag = flagInt.getInt() + 1;
			}
		});
	registerKeyword("color", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt colorInt(theStream);
			customColors.color = colorInt.getInt() + 1;
		});
	registerKeyword("symbol_index", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt siInt(theStream);
			customColors.symbolIndex = siInt.getInt() + 1;
		});
	registerKeyword("flag_colors", [this](const std::string& colorsString, std::istream& theStream)
		{
			customColors.flagColors = commonItems::Color(theStream);
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}

