#include "Culture.h"
#include "ParserHelpers.h"

mappers::Culture::Culture(std::istream& theStream)
{
	registerKeyword("primary", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString primaryStr(theStream);
			primaryTag = primaryStr.getString();
		});
	registerKeyword("graphical_culture", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString graphicStr(theStream);
			graphicalCulture = graphicStr.getString();
		});
	registerKeyword("male_names", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::stringList names(theStream);
			maleNames = names.getStrings();
		});
	registerKeyword("female_names", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::stringList names(theStream);
			femaleNames = names.getStrings();
		});
	registerKeyword("dynasty_names", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::stringList names(theStream);
			dynastyNames = names.getStrings();
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
