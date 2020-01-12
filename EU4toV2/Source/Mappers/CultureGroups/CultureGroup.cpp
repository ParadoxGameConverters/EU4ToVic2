#include "CultureGroup.h"
#include "ParserHelpers.h"

mappers::CultureGroup::CultureGroup(const std::string& name_, std::istream& theStream):
	name(name_)
{
	registerKeyword("graphical_culture", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString token(theStream);
			graphicalCulture = token.getString();
		});
	registerKeyword("male_names", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::stringList names(theStream);
			maleNames = names.getStrings();
		});
	registerKeyword("female_names", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::stringList names(theStream);
			femaleNames = names.getStrings();
		});
	registerKeyword("dynasty_names", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::stringList names(theStream);
			dynastyNames = names.getStrings();
		});
	registerRegex("\\w+", [this](const std::string& cultureName, std::istream& theStream)
		{
			Culture newCulture(theStream);
			cultures.insert(make_pair(cultureName, newCulture));
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
}
