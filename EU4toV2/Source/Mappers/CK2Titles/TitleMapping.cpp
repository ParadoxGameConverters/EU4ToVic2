#include "TitleMapping.h"
#include "ParserHelpers.h"
#include "OSCompatibilityLayer.h"


mappers::TitleMapping::TitleMapping(std::istream& theStream)
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString nameStr(theStream);
			name = Utils::convertUTF8To8859_15(nameStr.getString());
		});
	registerKeyword("title", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString titleStr(theStream);
			ID = titleStr.getString();
		});
	registerKeyword("region", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString regionStr(theStream);
			region = regionStr.getString();
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}

bool mappers::TitleMapping::hasIslamicRegion() const
{
	return region == "e_persia" || region == "e_arabia";
}

bool mappers::TitleMapping::hasIndianRegion() const
{
	return region == "e_rajastan" || region == "e_bengal" || region == "e_deccan";
}
