#include "Culture.h"
#include "ParserHelpers.h"

mappers::Culture::Culture(std::istream& theStream)
{
	registerKeyword(std::regex("country"), commonItems::ignoreItem);
	registerKeyword(std::regex("province"), commonItems::ignoreItem);
	registerKeyword(std::regex("primary"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString primaryTagStr(theStream);
			primaryTag = primaryTagStr.getString();
		});
	registerKeyword(std::regex("graphical_culture"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString graphStr(theStream);
			graphicalCulture = graphStr.getString();
		});
	registerKeyword(std::regex("male_names"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::stringList names(theStream);
			maleNames = names.getStrings();
		});
	registerKeyword(std::regex("female_names"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::stringList names(theStream);
			femaleNames = names.getStrings();
		});
	registerKeyword(std::regex("dynasty_names"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::stringList names(theStream);
			dynastyNames = names.getStrings();
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}
