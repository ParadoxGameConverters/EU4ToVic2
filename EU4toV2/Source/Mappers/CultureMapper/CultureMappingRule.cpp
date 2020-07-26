#include "CultureMappingRule.h"
#include "../../EU4World/Regions/Regions.h"
#include "Log.h"
#include "ParserHelpers.h"

mappers::CultureMappingRule::CultureMappingRule(std::istream& theStream)
{
	registerKeyword("vic2", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString v2Str(theStream);
		destinationCulture = v2Str.getString();
	});
	registerKeyword("region", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString regionStr(theStream);
		regions.insert(regionStr.getString());
	});
	registerKeyword("religion", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString religionStr(theStream);
		religions.insert(religionStr.getString());
	});
	registerKeyword("owner", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString ownerStr(theStream);
		owners.insert(ownerStr.getString());
	});
	registerKeyword("provinceid", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString provinceStr(theStream);
		try
		{
			provinces.insert(stoi(provinceStr.getString()));
		}
		catch (std::exception&)
		{
			Log(LogLevel::Warning) << "Invalid province ID in culture mapper: " << provinceStr.getString();
		}
	});
	registerKeyword("eu4", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString eu4Str(theStream);
		cultures.insert(eu4Str.getString());
	});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}

mappers::CultureMappingRule::CultureMappingRule(const std::string& v2Culture, const std::string& eu4Culture, const std::string& eu4SuperRegion):
	 destinationCulture(v2Culture)
{
	cultures.insert(eu4Culture);
	regions.insert(eu4SuperRegion);
}

std::optional<std::string> mappers::CultureMappingRule::cultureMatch(const EU4::Regions& eu4Regions,
	 const std::string& eu4culture,
	 const std::string& eu4religion,
	 int eu4Province,
	 const std::string& eu4ownerTag) const
{
	// We need at least a viable EU4culture.
	if (eu4culture.empty())
		return std::nullopt;

	if (!cultures.count(eu4culture))
		return std::nullopt;

	if (!eu4ownerTag.empty() && !owners.empty())
		if (!owners.count(eu4ownerTag))
			return std::nullopt;

	if (!eu4religion.empty() && !religions.empty())
		if (!religions.count(eu4religion))
			return std::nullopt;

	// This is a provinces check, not regions.
	if (eu4Province && !provinces.empty())
		if (!provinces.count(eu4Province))
			return std::nullopt;

	// This is a regions check, that checks if a provided province is within that region.
	if (eu4Province && !regions.empty())
	{
		auto regionMatch = false;
		for (const auto& region: regions)
		{
			if (!eu4Regions.regionIsValid(region))
			{
				// Regions change between versions so don't react to invalid region name.
				continue;
			}
			if (eu4Regions.provinceInRegion(eu4Province, region))
				regionMatch = true;
		}
		if (!regionMatch)
			return std::nullopt;
	}
	return destinationCulture;
}

std::optional<std::string> mappers::CultureMappingRule::cultureRegionalMatch(const EU4::Regions& eu4Regions,
	 const std::string& eu4culture,
	 const std::string& eu4religion,
	 int eu4Province,
	 const std::string& eu4ownerTag) const
{
	// This is a regional match. We need a mapping within the given region, so if the
	// mapping rule has no regional qualifiers it needs to fail.
	if (regions.empty())
		return std::nullopt;

	// Otherwise, as usual.
	return cultureMatch(eu4Regions, eu4culture, eu4religion, eu4Province, eu4ownerTag);
}

std::optional<std::string> mappers::CultureMappingRule::cultureNonRegionalNonReligiousMatch(const EU4::Regions& eu4Regions,
	 const std::string& eu4culture,
	 const std::string& eu4religion,
	 int eu4Province,
	 const std::string& eu4ownerTag) const
{
	// This is a non regional non religious match. We need a mapping without any region/religion, so if the
	// mapping rule has any regional/religious qualifiers it needs to fail.
	if (!regions.empty())
		return std::nullopt;
	if (!religions.empty())
		return std::nullopt;

	// Otherwise, as usual.
	return cultureMatch(eu4Regions, eu4culture, eu4religion, eu4Province, eu4ownerTag);
}
