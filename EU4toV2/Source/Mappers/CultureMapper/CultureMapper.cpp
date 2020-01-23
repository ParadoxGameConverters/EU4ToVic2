#include "CultureMapper.h"
#include "CultureMappingRule.h"
#include "ParserHelpers.h"

mappers::CultureMapper::CultureMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::CultureMapper::loadFile(const std::string& fileName)
{
	registerKeys();
	parseFile(fileName);
	clearRegisteredKeywords();
}

void mappers::CultureMapper::registerKeys()
{
	registerKeyword("link", [this](const std::string& unused, std::istream& theStream)
		{
			CultureMappingRule rule(theStream);
			cultureMapRules.push_back(rule);
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}

std::optional<std::string> mappers::CultureMapper::cultureMatch(
	const EU4::Regions& EU4Regions,
	const std::string& EU4culture,
	const std::string& EU4religion,
	int EU4Province,
	const std::string& EU4ownerTag
) const
{
	for (const auto& cultureMappingRule: cultureMapRules)
	{
		const auto& possibleMatch = cultureMappingRule.cultureMatch(EU4Regions, EU4culture, EU4religion, EU4Province, EU4ownerTag);
		if (possibleMatch) return *possibleMatch;
	}
	return std::nullopt;
}