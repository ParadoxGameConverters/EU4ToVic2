#include "TechGroups.h"
#include "ParserHelpers.h"

mappers::TechGroups::TechGroups(std::istream& theStream)
{
	registerKeyword("westernization", [this](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleInt westernizationInt(theStream);
			westernization = westernizationInt.getInt();
		});
	registerKeyword("literacy", [this](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleInt literacyInt(theStream);
			literacyBoost = literacyInt.getInt();
		});

	parseStream(theStream);
	clearRegisteredKeywords();
}