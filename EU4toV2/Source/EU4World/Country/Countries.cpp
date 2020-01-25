#include "Countries.h"
#include "ParserHelpers.h"

EU4::Countries::Countries(
	const Version& theVersion,
	std::istream& theStream,
	const mappers::IdeaEffectMapper& ideaEffectMapper, 
	const mappers::CultureGroups& cultureGroupsMapper)
{
	registerKeyword("---", commonItems::ignoreItem);
	registerKeyword("REB", commonItems::ignoreItem);
	registerKeyword("PIR", commonItems::ignoreItem);
	registerKeyword("NAT", commonItems::ignoreItem);
	registerRegex("[A-Z]{3}|[A-Z][0-9]{2}", [this, theVersion, ideaEffectMapper, cultureGroupsMapper](const std::string& tag, std::istream& theStream)
		{
			auto country = std::make_shared<Country>(tag, theVersion, theStream, ideaEffectMapper, cultureGroupsMapper);
			theCountries.insert(std::make_pair(tag, country));
		}
	);
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}