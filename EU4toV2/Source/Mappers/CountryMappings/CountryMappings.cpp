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
#include "../../V2World/V2Country.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include "../../V2World/Localisation/Localisation.h"

mappers::CountryMappings::CountryMappings()
{
	LOG(LogLevel::Info) << "Getting country mappings";
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
			CountryMapping newMapping(theStream);
			EU4TagToV2TagsRules.insert(make_pair(newMapping.getEU4Tag(), newMapping.getVic2Tag()));
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);
}

void mappers::CountryMappings::getAvailableFlags()
{
	LOG(LogLevel::Info) << "\tCataloguing available flags";
	const std::vector<std::string> availableFlagFolders = { "blankMod/output/gfx/flags", theConfiguration.getVic2Path() + "/gfx/flags" };

	std::set<std::string> availableFlagFiles;
	for (auto availableFlagFolder: availableFlagFolders)
	{
		Utils::GetAllFilesInFolder(availableFlagFolder, availableFlagFiles);
	}

	for (auto file: availableFlagFiles)
	{
		size_t lastdot = file.find_last_of(".");
		if (lastdot != std::string::npos)
		{
			availableFlags.insert(file.substr(0, lastdot)); 
		}
	}
}

void mappers::CountryMappings::createMappings(
	const EU4::World& srcWorld,
	const std::map<std::string, V2::V2Country*>& Vic2Countries,
	const ProvinceMapper& provinceMapper
) {
	LOG(LogLevel::Info) << "Creating country mappings";

	std::set<std::shared_ptr<EU4::Country>> colonialCountries;
	for (auto EU4Country: srcWorld.getCountries())
	{
		if (isPotentialColonialReplacement(EU4Country))
		{
			colonialCountries.insert(EU4Country.second);
		}
		else
		{
			makeOneMapping(EU4Country.second, Vic2Countries);
		}
	}

	for (auto colonialCountry: colonialCountries)
	{
		bool success = attemptColonialReplacement(colonialCountry, srcWorld, Vic2Countries, provinceMapper);
		if (!success)
		{
			makeOneMapping(colonialCountry, Vic2Countries);
		}
	}
}

bool mappers::CountryMappings::isPotentialColonialReplacement(const std::pair<std::string, std::shared_ptr<EU4::Country>>& country)
{
	if (country.second->isColony() && tagIsAlphaDigitDigit(country.first))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool mappers::CountryMappings::tagIsAlphaDigitDigit(const std::string& tag) const
{
	return (isalpha(tag[0]) && isdigit(tag[1]) && isdigit(tag[2]));
}

void mappers::CountryMappings::makeOneMapping(std::shared_ptr<EU4::Country> country, const std::map<std::string, V2::V2Country*>& Vic2Countries)
{
	std::string EU4Tag = country->getTag();

	auto mappingRule = EU4TagToV2TagsRules.find(EU4Tag);
	mappingRule = ifValidGetCK2MappingRule(country, mappingRule);

	bool mapped = false;
	if ((mappingRule != EU4TagToV2TagsRules.end()) && (!country->isCustom()))
	{
		mapped = mapToExistingVic2Country(mappingRule->second, Vic2Countries, EU4Tag);
		if (!mapped)
		{
			mapped = mapToFirstUnusedVic2Tag(mappingRule->second, EU4Tag);
		}
	}

	if (!mapped)
	{
		std::string newVic2Tag = generateNewTag();
		mapToNewTag(EU4Tag, newVic2Tag);
	}
}

bool mappers::CountryMappings::mapToExistingVic2Country(const std::string& possibleVic2Tag, const std::map<std::string, V2::V2Country*>& Vic2Countries, const std::string& EU4Tag)
{
	if ((Vic2Countries.find(possibleVic2Tag) != Vic2Countries.end()) && (!tagIsAlreadyAssigned(possibleVic2Tag)))
	{
		EU4TagToV2TagMap.insert(std::make_pair(EU4Tag, possibleVic2Tag));
		V2TagToEU4TagMap.insert(std::make_pair(possibleVic2Tag, EU4Tag));

		return true;
	}

	return false;
}

bool mappers::CountryMappings::mapToFirstUnusedVic2Tag(const std::string& possibleVic2Tag, const std::string& EU4Tag)
{
	if (!tagIsAlreadyAssigned(possibleVic2Tag))
	{
		EU4TagToV2TagMap.insert(std::make_pair(EU4Tag, possibleVic2Tag));
		V2TagToEU4TagMap.insert(std::make_pair(possibleVic2Tag, EU4Tag));

		return true;
	}

	return false;
}

std::string mappers::CountryMappings::generateNewTag()
{
	std::ostringstream generatedV2TagStream;
	generatedV2TagStream << generatedV2TagPrefix << std::setfill('0') << std::setw(2) << generatedV2TagSuffix;
	std::string Vic2Tag = generatedV2TagStream.str();

	++generatedV2TagSuffix;
	if (generatedV2TagSuffix > 99)
	{
		generatedV2TagSuffix = 0;
		--generatedV2TagPrefix;
	}

	return Vic2Tag;
}

void mappers::CountryMappings::mapToNewTag(const std::string& EU4Tag, const std::string& Vic2Tag)
{
	EU4TagToV2TagMap.insert(std::make_pair(EU4Tag, Vic2Tag));
	V2TagToEU4TagMap.insert(std::make_pair(Vic2Tag, EU4Tag));
	logMapping(EU4Tag, Vic2Tag, "generated tag");
}

std::map<std::string, std::string>::iterator mappers::CountryMappings::ifValidGetCK2MappingRule(const std::shared_ptr<EU4::Country> country, std::map<std::string, std::string>::iterator mappingRule)
{
	if ((mappingRule == EU4TagToV2TagsRules.end()) || (country->isCustom()))
	{
		auto CK2Title = getCK2Title(country->getTag(), country->getName("english"), availableFlags);
		if (CK2Title)
		{
			std::transform(CK2Title->begin(), CK2Title->end(), CK2Title->begin(), ::toupper);
			mappingRule = EU4TagToV2TagsRules.find(*CK2Title);
		}
	}

	return mappingRule;
}

bool mappers::CountryMappings::attemptColonialReplacement(
	std::shared_ptr<EU4::Country> country,
	const EU4::World& srcWorld,
	const std::map<std::string, V2::V2Country*>& Vic2Countries,
	const ProvinceMapper& provinceMapper
) {
	std::optional<int> Vic2Capital;
	int EU4Capital = country->getCapital();
	auto potentialVic2Capitals = provinceMapper.getVic2ProvinceNumbers(EU4Capital);
	if (potentialVic2Capitals.size() > 0)
	{
		Vic2Capital = *potentialVic2Capitals.begin();
	}

	for (auto colony: colonialTagMapper.getColonyList())
	{
		if (!capitalInRightEU4Region(colony, EU4Capital, provinceMapper))
		{
			continue;
		}
		country->setColonialRegion(colony.EU4Region);

		if (!capitalInRightVic2Region(colony, Vic2Capital, srcWorld, country->getTag(), provinceMapper))
		{
			continue;
		}

		if (!inCorrectCultureGroup(colony, country->getPrimaryCulture(), srcWorld.getCultureGroupsMapper()))
		{
			continue;
		}

		if (tagIsAvailable(colony, Vic2Countries))
		{
			EU4TagToV2TagMap.insert(make_pair(country->getTag(), colony.tag));
			V2TagToEU4TagMap.insert(make_pair(colony.tag, country->getTag()));
			logMapping(country->getTag(), colony.tag, "colonial replacement");
			return true;
		}
	}

	return false;
}

bool mappers::CountryMappings::capitalInRightEU4Region(const mappers::ColonyStruct& colony, int EU4Capital, const ProvinceMapper& provinceMapper)
{
	if (colony.EU4Region != "")
	{
		return provinceMapper.provinceIsInRegion(EU4Capital, colony.EU4Region);
	}
	else
	{
		return true;
	}
}

bool mappers::CountryMappings::capitalInRightVic2Region(
	const mappers::ColonyStruct& colony,
	std::optional<int> Vic2Capital,
	const EU4::World& srcWorld,
	const std::string& EU4Tag,
	const ProvinceMapper& provinceMapper
) {
	if (colony.V2Region != "")
	{
		if (Vic2Capital && regionProvinceMapper.provinceIsInRegion(*Vic2Capital, colony.V2Region))
		{
			return true;
		}
		else
		{
			for (auto Vic2ProvinceNumber: regionProvinceMapper.getProvincesInRegion(colony.V2Region))
			{
				auto EU4ProvinceNumbers = provinceMapper.getEU4ProvinceNumbers(Vic2ProvinceNumber);
				if (EU4ProvinceNumbers.size() > 0)
				{
					return false;
				}
				for (auto EU4ProvinceNumber: EU4ProvinceNumbers)
				{
					const EU4::Province& province = srcWorld.getProvince(EU4ProvinceNumber);
					if (province.getOwnerString() != EU4Tag)
					{
						return false;
					}
				}
			}
		}
	}

	return true;
}

bool mappers::CountryMappings::inCorrectCultureGroup(const mappers::ColonyStruct& colony, const std::string& primaryCulture, const mappers::CultureGroups& cultureGroupsMapper)
{
	if (colony.cultureGroup != "")
	{
		auto culturalGroup = cultureGroupsMapper.getCulturalGroup(primaryCulture);
		if ((culturalGroup) && (culturalGroup->getName() != colony.cultureGroup))
		{
			return false;
		}
	}

	return true;
}

bool mappers::CountryMappings::tagIsAvailable(const mappers::ColonyStruct& colony, const std::map<std::string, V2::V2Country*>& Vic2Countries)
{
	if (Vic2Countries.find(colony.tag) == Vic2Countries.end())
	{
		return false;
	}
	if (tagIsAlreadyAssigned(colony.tag))
	{
		return false;
	}

	return true;
}

void mappers::CountryMappings::logMapping(const std::string& EU4Tag, const std::string& V2Tag, const std::string& reason)
{
	LOG(LogLevel::Info) << "\tMapping " << EU4Tag << " -> " << V2Tag << " (" << reason << ')';
}

bool mappers::CountryMappings::tagIsAlreadyAssigned(const std::string& Vic2Tag)
{
	return (V2TagToEU4TagMap.find(Vic2Tag) != V2TagToEU4TagMap.end());
}

const std::string mappers::CountryMappings::getV2Tag(const std::string& EU4Tag) const
{
	const std::vector<std::string> EU4RebelTags = { "REB", "PIR", "NAT" };
	static const std::string V2RebelTag = "REB";
	if (find(EU4RebelTags.begin(), EU4RebelTags.end(), EU4Tag) != EU4RebelTags.end())
	{
		return V2RebelTag;
	}

	auto findIter = EU4TagToV2TagMap.find(EU4Tag);
	if (findIter != EU4TagToV2TagMap.end())
	{
		return findIter->second;
	}
	else
	{
		return "";
	}
}

std::optional<std::string> mappers::CountryMappings::getCK2Title(const std::string& EU4Tag, const std::string& countryName, const std::set<std::string>& availableFlags) const
{
	if (!tagIsAlphaDigitDigit(EU4Tag))
	{
		return {};
	}

	std::string name = V2::Localisation::convert(countryName);
	transform(name.begin(), name.end(), name.begin(), ::tolower);

	auto ck2title = ck2titleMapper.getTitle(name);
	if (!ck2title)
	{
		std::string titlename = V2::Localisation::stripAccents(name);
		std::string c_name = "c_" + titlename;
		std::string d_name = "d_" + titlename;
		std::string k_name = "k_" + titlename;

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
			// I've found titles that don't exist in the ck2 name mapping, but do exist in the flagset (c_znojmo).
			if (availableFlags.find("k_" + titlename) != availableFlags.end())
			{
				LOG(LogLevel::Debug) << "Country " << EU4Tag << " (" << name << ") has the CK2 title k_" << titlename;
				return k_name;
			}
			else if (availableFlags.find(d_name) != availableFlags.end())
			{
				LOG(LogLevel::Debug) << "Country " << EU4Tag << " (" << name << ") has the CK2 title " << d_name;
				return d_name;
			}
			else if (availableFlags.find(c_name) != availableFlags.end())
			{
				LOG(LogLevel::Debug) << "Country " << EU4Tag << " (" << name << ") has the CK2 title " << c_name;
				return c_name;
			}
		}
	}

	if (ck2title)
	{
		LOG(LogLevel::Debug) << "Country " << EU4Tag << " (" << name << ") has the CK2 title " << *ck2title;
	}

	return ck2title;
}