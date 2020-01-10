#include "CultureGroup.h"
#include "ParserHelpers.h"

mappers::CultureGroup::CultureGroup(const std::string& name_, std::istream& theStream)
{
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
	registerKeyword(std::regex("\\w+"), [this](const std::string& cultureName, std::istream& theStream)
		{
			mappers::Culture newCulture(theStream);
			cultures.insert(make_pair(cultureName, newCulture));
		});

	parseStream(theStream);
}
