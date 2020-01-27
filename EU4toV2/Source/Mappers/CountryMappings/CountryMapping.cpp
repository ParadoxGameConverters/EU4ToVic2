#include "CountryMapping.h"
#include "ParserHelpers.h"

mappers::CountryMapping::CountryMapping(std::istream& theStream)
{
	registerKeyword("EU4", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString eu4str(theStream);
			eu4Tag = eu4str.getString();
			std::transform(eu4Tag.begin(), eu4Tag.end(), eu4Tag.begin(), ::toupper);
		});
	registerKeyword("Vic2", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString v2str(theStream);
			vic2Tag = v2str.getString();
			std::transform(vic2Tag.begin(), vic2Tag.end(), vic2Tag.begin(), ::toupper);
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
