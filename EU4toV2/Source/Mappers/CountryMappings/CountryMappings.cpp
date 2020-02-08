#include "CountryMappings.h"
#include "CountryMapping.h"
#include <algorithm>
#include <iomanip>
#include "../../Configuration.h"
#include "../../EU4World/World.h"
#include "../../EU4World/Country/EU4Country.h"
#include "../../EU4World/Provinces/EU4Province.h"
#include "../CK2Titles/CK2TitleMapper.h"
#include "../ProvinceMappings/ProvinceMapper.h"
#include "../../V2World/Country/Country.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include "../../V2World/Localisation/Localisation.h"

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

	registerKeyword("link", [this](const std::string& unused, std::istream& theStream)
		{
			const CountryMapping newMapping(theStream);
			eu4TagToV2TagsRules.insert(make_pair(newMapping.getEU4Tag(), newMapping.getVic2Tag()));
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);
}

void mappers::CountryMappings::getAvailableFlags()
{
	LOG(LogLevel::Info) << "\tCataloguing available flags";
	const std::vector<std::string> availableFlagFolders = { "flags", theConfiguration.getVic2Path() + "/gfx/flags" };

	std::set<std::string> availableFlagFiles;
	for (const auto& availableFlagFolder: availableFlagFolders)
	{
		Utils::GetAllFilesInFolder(availableFlagFolder, availableFlagFiles);
	}

	for (const auto& file: availableFlagFiles)
	{
		const auto lastdot = file.find_last_of(".");
		if (lastdot != std::string::npos)
		{
			availableFlags.insert(file.substr(0, lastdot)); 
		}
	}
}

void mappers::CountryMappings::createMappings(
	const EU4::World& srcWorld,
	const std::map<std::string, std::shared_ptr<V2::Country>>& vic2Countries,
	const ProvinceMapper& provinceMapper
) {
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
		if (!success) makeOneMapping(*colonialCountry, vic2Countries);
	}
}

bool mappers::CountryMappings::isPotentialColonialReplacement(const std::pair<std::string, std::shared_ptr<EU4::Country>>& country)
{
	if (country.second->isColony() && tagIsAlphaDigitDigit(country.first)) return true;
	return false;
}

bool mappers::CountryMappings::tagIsAlphaDigitDigit(const std::string& tag)
{
	return isalpha(tag[0]) && isdigit(tag[1]) && isdigit(tag[2]);
}

void mappers::CountryMappings::makeOneMapping(const EU4::Country& country, const std::map<std::string, std::shared_ptr<V2::Country>>& vic2Countries)
{
	const auto& EU4Tag = country.getTag();

	auto mappingRule = eu4TagToV2TagsRules.find(EU4Tag);
	mappingRule = ifValidGetCK2MappingRule(country, mappingRule);

	auto mapped = false;
	if (mappingRule != eu4TagToV2TagsRules.end() && !country.isCustom())
	{
		mapped = mapToExistingVic2Country(mappingRule->second, vic2Countries, EU4Tag);
		if (!mapped) mapped = mapToFirstUnusedVic2Tag(mappingRule->second, EU4Tag);
	}

	if (!mapped)
	{
		const auto& newVic2Tag = generateNewTag();
		mapToNewTag(EU4Tag, newVic2Tag);
	}
}

bool mappers::CountryMappings::mapToExistingVic2Country(const std::string& possibleVic2Tag, const std::map<std::string, std::shared_ptr<V2::Country>>& vic2Countries, const std::string& eu4Tag)
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
	logMapping(eu4Tag, vic2Tag, "generated tag");
}

std::map<std::string, std::string>::iterator mappers::CountryMappings::ifValidGetCK2MappingRule(const EU4::Country& country, std::map<std::string, std::string>::iterator mappingRule)
{
	if (mappingRule == eu4TagToV2TagsRules.end() || country.isCustom())
	{
		auto CK2Title = getCK2Title(country.getTag(), country.getName("english"), availableFlags);
		if (CK2Title)
		{
			std::transform(CK2Title->begin(), CK2Title->end(), CK2Title->begin(), ::toupper);
			mappingRule = eu4TagToV2TagsRules.find(*CK2Title);
		}
	}

	return mappingRule;
}

bool mappers::CountryMappings::attemptColonialReplacement(
	EU4::Country& country,
	const EU4::World& srcWorld,
	const std::map<std::string, std::shared_ptr<V2::Country>>& vic2Countries,
	const ProvinceMapper& provinceMapper
) {
	std::optional<int> vic2Capital;
	const auto EU4Capital = country.getCapital();
	auto potentialVic2Capitals = provinceMapper.getVic2ProvinceNumbers(EU4Capital);
	if (!potentialVic2Capitals.empty()) vic2Capital = *potentialVic2Capitals.begin();

	for (const auto& colony: colonialTagMapper.getColonyList())
	{
		if (!capitalInRightEU4Region(colony, EU4Capital, provinceMapper)) continue;
		country.setColonialRegion(colony.EU4Region);

		if (!capitalInRightVic2Region(colony, vic2Capital, srcWorld, country.getTag(), provinceMapper)) continue;

		if (!inCorrectCultureGroup(colony, country.getPrimaryCulture(), srcWorld.getCultureGroupsMapper())) continue;

		if (tagIsAvailable(colony, vic2Countries))
		{
			eu4TagToV2TagMap.insert(make_pair(country.getTag(), colony.tag));
			v2TagToEU4TagMap.insert(make_pair(colony.tag, country.getTag()));
			logMapping(country.getTag(), colony.tag, "colonial replacement");
			return true;
		}
	}
	return false;
}

bool mappers::CountryMappings::capitalInRightEU4Region(const ColonyStruct& colony, int eu4Capital, const ProvinceMapper& provinceMapper)
{
	if (!colony.EU4Region.empty()) return provinceMapper.provinceIsInRegion(eu4Capital, colony.EU4Region);
	return true;
}

bool mappers::CountryMappings::capitalInRightVic2Region(
	const ColonyStruct& colony,
	std::optional<int> vic2Capital,
	const EU4::World& srcWorld,
	const std::string& eu4Tag,
	const ProvinceMapper& provinceMapper) const
{
	if (colony.V2Region.empty()) return true;
	if (vic2Capital && regionProvinceMapper.provinceIsInRegion(*vic2Capital, colony.V2Region)) return true;

	for (auto vic2ProvinceNumber: regionProvinceMapper.getProvincesInRegion(colony.V2Region))
	{
		auto eu4ProvinceNumbers = provinceMapper.getEU4ProvinceNumbers(vic2ProvinceNumber);
		if (!eu4ProvinceNumbers.empty()) return false;
		for (auto eu4ProvinceNumber: eu4ProvinceNumbers)
		{
			if (srcWorld.getProvince(eu4ProvinceNumber)->getOwnerString() != eu4Tag) return false;
		}
	}
	return true;
}

bool mappers::CountryMappings::inCorrectCultureGroup(const ColonyStruct& colony, const std::string& primaryCulture, const CultureGroups& cultureGroupsMapper)
{
	if (!colony.cultureGroup.empty())
	{
		const auto& culturalGroup = cultureGroupsMapper.getGroupForCulture(primaryCulture);
		if (culturalGroup && culturalGroup->getName() != colony.cultureGroup) return false;
	}
	return true;
}

bool mappers::CountryMappings::tagIsAvailable(const ColonyStruct& colony, const std::map<std::string, std::shared_ptr<V2::Country>>& vic2Countries) const
{
	if (vic2Countries.find(colony.tag) == vic2Countries.end()) return false;
	if (tagIsAlreadyAssigned(colony.tag)) return false;
	return true;
}

void mappers::CountryMappings::logMapping(const std::string& eu4Tag, const std::string& v2Tag, const std::string& reason)
{
	LOG(LogLevel::Info) << "\tMapping " << eu4Tag << " -> " << v2Tag << " (" << reason << ')';
}

bool mappers::CountryMappings::tagIsAlreadyAssigned(const std::string& vic2Tag) const
{
	return v2TagToEU4TagMap.find(vic2Tag) != v2TagToEU4TagMap.end();
}

std::optional<std::string> mappers::CountryMappings::getV2Tag(const std::string& eu4Tag) const
{
	const std::vector<std::string> eu4RebelTags = { "REB", "PIR", "NAT" };
	static const std::string v2RebelTag = "REB";
	if (find(eu4RebelTags.begin(), eu4RebelTags.end(), eu4Tag) != eu4RebelTags.end())
	{
		return v2RebelTag;
	}

	const auto& findIter = eu4TagToV2TagMap.find(eu4Tag);
	if (findIter != eu4TagToV2TagMap.end()) return findIter->second;
	return std::nullopt;
}

std::optional<std::string> mappers::CountryMappings::getCK2Title(
	const std::string& eu4Tag, 
	const std::string& countryName, 
	const std::set<std::string>& availableFlags) const
{
	if (!tagIsAlphaDigitDigit(eu4Tag)) return std::nullopt;

	auto name = V2::Localisation::convert(countryName);
	transform(name.begin(), name.end(), name.begin(), ::tolower);

	auto ck2title = ck2titleMapper.getTitle(name);
	if (!ck2title)
	{
		const auto& titlename = V2::Localisation::stripAccents(name);
		const auto& c_name = "c_" + titlename;
		const auto& d_name = "d_" + titlename;
		const auto& k_name = "k_" + titlename;

		if (ck2titleMapper.doesTitleExist(c_name))
		{
			ck2title = c_name;
		}
		else if (ck2titleMapper.doesTitleExist(d_name))
		{
			ck2title = d_name;
		}
		else if (ck2titleMapper.doesTitleExist(k_name))
		{
			ck2title = k_name;
		}
		else
		{
			// Mayle titles don't exist in the ck2 name mapping, but do exist in the flagset (c_znojmo).
			if (availableFlags.find("k_" + titlename) != availableFlags.end()) return k_name;
			if (availableFlags.find(d_name) != availableFlags.end()) return d_name;
			if (availableFlags.find(c_name) != availableFlags.end()) return c_name;
		}
	}
	return ck2title;
}