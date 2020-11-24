#include "CountryMappings.h"
#include "../../Configuration.h"
#include "../../EU4World/Country/EU4Country.h"
#include "../../EU4World/World.h"
#include "../../V2World/Country/Country.h"
#include "../../V2World/Localisation/Localisation.h"
#include "../Titles/TitleMapper.h"
#include "../ProvinceMappings/ProvinceMapper.h"
#include "CountryMapping.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include <algorithm>
#include <iomanip>
#include "CommonFunctions.h"

mappers::CountryMappings::CountryMappings()
{
	LOG(LogLevel::Info) << "Parsing country mappings";
	registerKeys();
	parseFile("configurables/country_mappings.txt");
	clearRegisteredKeywords();
	getAvailableFlags();
}

mappers::CountryMappings::CountryMappings(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
	getAvailableFlags();
}

void mappers::CountryMappings::registerKeys()
{
	LOG(LogLevel::Info) << "\tReading country mapping rules";

	registerKeyword("link", [this](const std::string& unused, std::istream& theStream) {
		const CountryMapping newMapping(theStream);
		if (!newMapping.getEU4Tag().empty())
			eu4TagToV2TagsRules.emplace_back(std::make_pair(newMapping.getEU4Tag(), newMapping));
		else
			eu4TagToV2TagsRules.emplace_back(std::make_pair("---custom---", newMapping));
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void mappers::CountryMappings::getAvailableFlags()
{
	LOG(LogLevel::Info) << "\tCataloguing available flags";
	const std::vector<std::string> availableFlagFolders = {"flags", theConfiguration.getVic2Path() + "/gfx/flags"};

	for (const auto& availableFlagFolder: availableFlagFolders)
	{
		auto availableFlagFiles = commonItems::GetAllFilesInFolder(availableFlagFolder);
		for (const auto& file: availableFlagFiles)
			availableFlags.insert(trimExtension(file));
	}
}

void mappers::CountryMappings::createMappings(const EU4::World& srcWorld,
	 const std::map<std::string, std::shared_ptr<V2::Country>>& vic2Countries,
	 const ProvinceMapper& provinceMapper)
{
	LOG(LogLevel::Info) << "Creating country mappings";

	std::set<std::shared_ptr<EU4::Country>> colonialCountries;
	for (const auto& EU4Country: srcWorld.getCountries())
	{
		if (isPotentialColonialReplacement(EU4Country))
		{
			colonialCountries.insert(EU4Country.second);
		}
		else
		{
			makeOneMapping(*EU4Country.second, vic2Countries);
		}
	}

	for (const auto& colonialCountry: colonialCountries)
	{
		const auto& success = attemptColonialReplacement(*colonialCountry, srcWorld, vic2Countries, provinceMapper);
		if (!success)
			makeOneMapping(*colonialCountry, vic2Countries);
	}
}

bool mappers::CountryMappings::isPotentialColonialReplacement(const std::pair<std::string, std::shared_ptr<EU4::Country>>& country)
{
	if (country.second->isColony() && tagIsAlphaDigitDigit(country.first))
		return true;
	return false;
}

bool mappers::CountryMappings::tagIsAlphaDigitDigit(const std::string& tag)
{
	return isalpha(tag[0]) && isdigit(tag[1]) && isdigit(tag[2]);
}

void mappers::CountryMappings::makeOneMapping(const EU4::Country& country, const std::map<std::string, std::shared_ptr<V2::Country>>& vic2Countries)
{
	const auto& EU4Tag = country.getTag();

	auto mapped = attemptStraightMapping(country, vic2Countries, EU4Tag);
	if (mapped)
		return;

	// There was no functional mapping in country_mapping.txt. Let's see if we can generate a ck2 title from the country's name
	// that we can map into vic2.

	const auto& potentialCK2Title = determineMappableCK2Title(country);
	if (potentialCK2Title)
		mapped = attemptStraightMapping(country, vic2Countries, *potentialCK2Title);
	if (mapped)
		return;

	// with no CK2 fallback, or a custom country, generate own tag.
	const auto& newVic2Tag = generateNewTag();
	mapToNewTag(EU4Tag, newVic2Tag);
}

bool mappers::CountryMappings::attemptStraightMapping(const EU4::Country& country,
	 const std::map<std::string, std::shared_ptr<V2::Country>>& vic2Countries,
	 const std::string& EU4Tag)
{
	auto mapped = false;
	for (const auto& mappingLine: eu4TagToV2TagsRules)
	{
		if (mappingLine.first != "---custom---" && mappingLine.first != EU4Tag)
			continue;
		if (!mappingLine.second.getReforms().empty())
		{
			auto found = false;
			for (const auto& requiredReform: mappingLine.second.getReforms())
			{
				if (country.hasReform(requiredReform))
					found = true;
			}
			if (!found)
				continue;
		}
		if (!mappingLine.second.getFlags().empty())
		{
			auto found = false;
			for (const auto& requiredFlag: mappingLine.second.getFlags())
			{
				if (country.hasFlag(requiredFlag))
					found = true;
			}
			if (!found)
				continue;
		}
		// We have found a solid mapping candidate among existing definitions. Mapping to a live country first.
		mapped = mapToExistingVic2Country(mappingLine.second.getVic2Tag(), vic2Countries, EU4Tag);
		// Maybe a dead country?
		if (!mapped)
			mapped = mapToFirstUnusedVic2Tag(mappingLine.second.getVic2Tag(), EU4Tag);
		if (mapped)
			return mapped;
	}
	return mapped;
}

bool mappers::CountryMappings::mapToExistingVic2Country(const std::string& possibleVic2Tag,
	 const std::map<std::string, std::shared_ptr<V2::Country>>& vic2Countries,
	 const std::string& eu4Tag)
{
	if (vic2Countries.find(possibleVic2Tag) != vic2Countries.end() && !tagIsAlreadyAssigned(possibleVic2Tag))
	{
		eu4TagToV2TagMap.insert(std::make_pair(eu4Tag, possibleVic2Tag));
		v2TagToEU4TagMap.insert(std::make_pair(possibleVic2Tag, eu4Tag));
		return true;
	}
	return false;
}

bool mappers::CountryMappings::mapToFirstUnusedVic2Tag(const std::string& possibleVic2Tag, const std::string& eu4Tag)
{
	if (!tagIsAlreadyAssigned(possibleVic2Tag))
	{
		eu4TagToV2TagMap.insert(std::make_pair(eu4Tag, possibleVic2Tag));
		v2TagToEU4TagMap.insert(std::make_pair(possibleVic2Tag, eu4Tag));
		return true;
	}

	return false;
}

std::string mappers::CountryMappings::generateNewTag()
{
	std::ostringstream generatedV2TagStream;
	generatedV2TagStream << generatedV2TagPrefix << std::setfill('0') << std::setw(2) << generatedV2TagSuffix;
	const auto& vic2Tag = generatedV2TagStream.str();

	++generatedV2TagSuffix;
	if (generatedV2TagSuffix > 99)
	{
		generatedV2TagSuffix = 0;
		--generatedV2TagPrefix;
	}

	return vic2Tag;
}

void mappers::CountryMappings::mapToNewTag(const std::string& eu4Tag, const std::string& vic2Tag)
{
	eu4TagToV2TagMap.insert(std::make_pair(eu4Tag, vic2Tag));
	v2TagToEU4TagMap.insert(std::make_pair(vic2Tag, eu4Tag));
}

std::optional<std::string> mappers::CountryMappings::determineMappableCK2Title(const EU4::Country& country)
{
	if (country.isCustom())
		return std::nullopt; // Custom countries must get generated V2 tags.

	auto title = getTitle(country.getTag(), country.getName("english"), availableFlags);
	if (!title)
		return std::nullopt;

	for (const auto& potentialMapping: eu4TagToV2TagsRules)
	{
		if (potentialMapping.first == *title)
			return *title;
	}

	return std::nullopt;
}

bool mappers::CountryMappings::attemptColonialReplacement(EU4::Country& country,
	 const EU4::World& srcWorld,
	 const std::map<std::string, std::shared_ptr<V2::Country>>& vic2Countries,
	 const ProvinceMapper& provinceMapper)
{
	const auto EU4Capital = country.getCapital();
	std::string eu4Region;
	if (const auto& potentialEU4Region = provinceMapper.getColonialRegionForProvince(EU4Capital); potentialEU4Region)
		eu4Region = *potentialEU4Region;

	int vic2Capital = 0;	
	if (const auto& potentialVic2Capitals = provinceMapper.getVic2ProvinceNumbers(EU4Capital); !potentialVic2Capitals.empty())
		vic2Capital = *potentialVic2Capitals.begin();
	std::string vic2Region;
	if (const auto& v2Region = v2provinceRegionsMapper.getRegionForProvince(vic2Capital); v2Region)
		vic2Region = *v2Region;

	std::string cultureGroup;
	if (const auto& culturalGroup = srcWorld.getCultureGroupsMapper().getGroupForCulture(country.getPrimaryCulture()); culturalGroup)
		cultureGroup = culturalGroup->getName();
		
	if (const auto& colonyMatch = colonialTagMapper.getColonialTag(eu4Region, vic2Region, cultureGroup); colonyMatch)
	{
		country.setColonialRegion(eu4Region);
		if (tagIsAvailable(*colonyMatch, vic2Countries))
		{
			eu4TagToV2TagMap.insert(make_pair(country.getTag(), *colonyMatch));
			v2TagToEU4TagMap.insert(make_pair(*colonyMatch, country.getTag()));
			return true;
		}
		else
			return false;
	}
	else 
		return false;
}

bool mappers::CountryMappings::tagIsAvailable(const std::string& colonyTag, const std::map<std::string, std::shared_ptr<V2::Country>>& vic2Countries) const
{
	if (vic2Countries.find(colonyTag) == vic2Countries.end())
		return false;
	if (tagIsAlreadyAssigned(colonyTag))
		return false;
	return true;
}

bool mappers::CountryMappings::tagIsAlreadyAssigned(const std::string& vic2Tag) const
{
	return v2TagToEU4TagMap.find(vic2Tag) != v2TagToEU4TagMap.end();
}

std::optional<std::string> mappers::CountryMappings::getV2Tag(const std::string& eu4Tag) const
{
	const std::vector<std::string> eu4RebelTags = {"REB", "PIR", "NAT"};
	static const std::string v2RebelTag = "REB";
	if (find(eu4RebelTags.begin(), eu4RebelTags.end(), eu4Tag) != eu4RebelTags.end())
	{
		return v2RebelTag;
	}

	const auto& findIter = eu4TagToV2TagMap.find(eu4Tag);
	if (findIter != eu4TagToV2TagMap.end())
		return findIter->second;
	return std::nullopt;
}

std::optional<std::string> mappers::CountryMappings::getTitle(const std::string& eu4Tag,
	 const std::string& countryName,
	 const std::set<std::string>& availableFlags) const
{

	auto name = V2::Localisation::convert(countryName);
	transform(name.begin(), name.end(), name.begin(), ::tolower);

	auto title = titleMapper.getTitleForName(name);
	if (!title)
	{
		const auto& titlename = V2::Localisation::stripAccents(name);
		const auto& c_name = "c_" + titlename;
		const auto& d_name = "d_" + titlename;
		const auto& k_name = "k_" + titlename;
		const auto& e_name = "e_" + titlename;

		if (titleMapper.doesTitleExist(c_name))
		{
			title = c_name;
		}
		else if (titleMapper.doesTitleExist(d_name))
		{
			title = d_name;
		}
		else if (titleMapper.doesTitleExist(k_name))
		{
			title = k_name;
		}
		else if (titleMapper.doesTitleExist(e_name))
		{
			title = e_name;
		}
		else
		{
			// Maybe titles don't exist in the ck2 name mapping, but do exist in the flagset (c_znojmo).
			if (availableFlags.find(e_name) != availableFlags.end())
				return e_name;
			if (availableFlags.find(k_name) != availableFlags.end())
				return k_name;
			if (availableFlags.find(d_name) != availableFlags.end())
				return d_name;
			if (availableFlags.find(c_name) != availableFlags.end())
				return c_name;
		}
	}
	return title;
}