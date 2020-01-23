#include "CultureMappingRule.h"
#include "ParserHelpers.h"
#include "../../EU4World/Regions/Regions.h"

mappers::CultureMappingRule::CultureMappingRule(std::istream& theStream)
{
	registerKeyword("vic2", [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::singleString v2Str(theStream);
			destinationCulture = v2Str.getString();
		});
	registerKeyword("region", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString regionStr(theStream);
			regions.insert(regionStr.getString());
		});
	registerKeyword("religion", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString religionStr(theStream);
			religions.insert(religionStr.getString());
		});
	registerKeyword("owner", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString ownerStr(theStream);
			owners.insert(ownerStr.getString());
		});
	registerKeyword("provinceid", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString provinceStr(theStream);
			if (stoi(provinceStr.getString())) provinces.insert(stoi(provinceStr.getString()));
		});
	registerKeyword("eu4", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString eu4Str(theStream);
			cultures.insert(eu4Str.getString());
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}

std::optional<std::string> mappers::CultureMappingRule::cultureMatch(
	const EU4::Regions& EU4Regions,
	const std::string& EU4culture,
	const std::string& EU4religion,
	int EU4Province,
	const std::string& EU4ownerTag
) const
{
	// We need at least a viable EU4culture.
	if (EU4culture.empty()) return std::nullopt;
		
	auto match = true;
	if (!cultures.count(EU4culture)) match = false;
	if (!EU4ownerTag.empty() && !owners.empty()) if (!owners.count(EU4ownerTag)) match = false;
	if (!EU4religion.empty() && !religions.empty()) if (!religions.count(EU4religion)) match = false;
	// This is a provinces check, not regions.
	if (EU4Province && !provinces.empty()) if (!provinces.count(EU4Province)) match = false;
	// This is a regions check, that checks if a provided province is within that region.
	if (EU4Province && !regions.empty())
	{
		auto regionMatch = false;
		for (const auto& region: regions)
		{
			if (EU4Regions.provinceInRegion(EU4Province, region)) regionMatch = true;
		}
		if (!regionMatch) match = false;
	}
	if (match) return destinationCulture;
	return std::nullopt;
}
