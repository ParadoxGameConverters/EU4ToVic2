#include "CulturalUnion.h"
#include "ParserHelpers.h"

mappers::CulturalUnion::CulturalUnion(std::istream& theStream)
{
	registerKeyword("culture", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString cultureString(theStream);
			culture = cultureString.getString();
		});
	registerKeyword("tag", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString tagString(theStream);
			tags.push_back(tagString.getString());
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
	theUnion = std::make_pair(culture, tags);
}