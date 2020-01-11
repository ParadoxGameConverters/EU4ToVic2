#include "Countries.h"
#include "ParserHelpers.h"

EU4::countries::countries(const EU4::Version& theVersion,std::istream& theStream,const mappers::IdeaEffectMapper& ideaEffectMapper)
{
	registerKeyword(std::regex("---"), commonItems::ignoreItem);
	registerKeyword(std::regex("REB"), commonItems::ignoreItem);
	registerKeyword(std::regex("PIR"), commonItems::ignoreItem);
	registerKeyword(std::regex("NAT"), commonItems::ignoreItem);
	registerKeyword(std::regex("[A-Z]{3}|[A-Z][0-9]{2}"), [this, theVersion, ideaEffectMapper](const std::string& tag, std::istream& theStream)
		{
			auto country = std::make_shared<EU4::Country>(tag, theVersion, theStream, ideaEffectMapper);
			theCountries.insert(std::make_pair(tag, country));
		}
	);
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}