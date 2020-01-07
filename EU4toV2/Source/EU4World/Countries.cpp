#include "Countries.h"
#include "EU4Country.h"
#include "../Mappers/Ideas/IdeaEffectMapper.h"
#include "ParserHelpers.h"

EU4::countries::countries(
	const EU4::Version& theVersion,
	std::istream& theStream,
	const mappers::IdeaEffectMapper& ideaEffectMapper
): theCountries()
{
	registerKeyword(std::regex("---"), commonItems::ignoreObject);
	registerKeyword(std::regex("REB"), commonItems::ignoreObject);
	registerKeyword(std::regex("PIR"), commonItems::ignoreObject);
	registerKeyword(std::regex("NAT"), commonItems::ignoreObject);
	registerKeyword(std::regex("[A-Z]{3}"), [this, theVersion, ideaEffectMapper](const std::string& tag, std::istream& theStream)
		{
			auto country = std::make_shared<EU4::Country>(tag, theVersion, theStream, ideaEffectMapper);
			theCountries.insert(std::make_pair(country->getTag(), country));
		}
	);
	registerKeyword(std::regex("[A-Z][0-9]{2}"),[this, theVersion, ideaEffectMapper](const std::string& tag, std::istream& theStream)
		{
			auto country = std::make_shared<EU4::Country>(tag, theVersion, theStream, ideaEffectMapper);
			theCountries.insert(std::make_pair(country->getTag(), country));
		}
	);

	parseStream(theStream);
}