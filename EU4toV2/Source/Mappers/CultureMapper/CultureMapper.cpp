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
	registerKeyword(("link"), [this](const std::string& unused, std::istream& theStream)
		{
			CultureMappingRule rule(theStream);
			for (auto newRule : rule.getMappings())
			{
				cultureMap.push_back(newRule);
			}
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}

std::optional<std::string> mappers::CultureMapper::cultureMatch(
	const EU4::Regions& EU4Regions,
	const std::string& culture,
	const std::string& religion,
	int EU4Province,
	const std::string& ownerTag
) const
{
	for (auto cultureMapping: cultureMap)
	{
		auto possibleMatch = cultureMapping.cultureMatch(EU4Regions, culture, religion, EU4Province, ownerTag);
		if (possibleMatch)
		{
			return possibleMatch;
		}
	}

	return {};
}