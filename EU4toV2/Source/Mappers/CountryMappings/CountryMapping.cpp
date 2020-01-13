#include "CountryMapping.h"
#include "ParserHelpers.h"

mappers::CountryMapping::CountryMapping(std::istream& theStream)
{
	registerKeyword("EU4", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString eu4str(theStream);
			EU4Tag = eu4str.getString();
			std::transform(EU4Tag.begin(), EU4Tag.end(), EU4Tag.begin(), ::toupper);
		});
	registerKeyword("Vic2", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString v2str(theStream);
			Vic2Tag = v2str.getString();
			std::transform(Vic2Tag.begin(), Vic2Tag.end(), Vic2Tag.begin(), ::toupper);
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
