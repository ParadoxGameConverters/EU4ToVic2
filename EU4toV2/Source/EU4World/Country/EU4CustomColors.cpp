#include "EU4CustomColors.h"
#include "NewParserToOldParserConverters.h"
#include "ParserHelpers.h"
#include <algorithm>


EU4::CustomColors::CustomColors(std::istream& theStream)
{
	registerKeyword(std::regex("flag"), [this](const std::string& unused, std::istream& theStream)
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
		}
	);
	registerKeyword(std::regex("color"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt colorInt(theStream);
			customColors.color = colorInt.getInt() + 1;
		}
	);
	registerKeyword(std::regex("symbol_index"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt siInt(theStream);
			customColors.symbolIndex = siInt.getInt() + 1;
		}
	);
	registerKeyword(std::regex("flag_colors"), [this](const std::string& colorsString, std::istream& theStream)
		{
			customColors.flagColors = commonItems::Color(theStream);
		}
	);

	registerKeyword(std::regex("[a-z0-9\\_]+"), commonItems::ignoreItem);
	parseStream(theStream);
}

