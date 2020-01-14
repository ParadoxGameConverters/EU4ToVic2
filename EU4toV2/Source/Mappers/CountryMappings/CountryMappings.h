#ifndef COUNTRY_MAPPINGS_H
#define COUNTRY_MAPPINGS_H

#include <map>
#include <optional>
#include <set>
#include "../ColonialTags/ColonialTagsMapper.h"
#include "../ProvinceMappings/ProvinceMapper.h"
#include "../CultureGroups/CultureGroups.h"
#include "../CK2Titles/CK2TitleMapper.h"
#include "newParser.h"

namespace EU4
{
	class Country;
	class World;
}
class V2Country;

namespace mappers
{
	class CountryMappings: commonItems::parser
	{
		public:
			CountryMappings();
			CountryMappings(std::istream& theStream);
			void createMappings(const EU4::World& srcWorld, const std::map<std::string, V2Country*>& Vic2Countries, const ProvinceMapper& provinceMapper);
			const std::string getV2Tag(const std::string& EU4Tag) const;
			std::optional<std::string> getCK2Title(const std::string& EU4Tag, const std::string& countryName, const std::set<std::string>& availableFlags) const;
			const mappers::CK2TitleMapper& getCK2TitleMapper() const { return ck2titleMapper; }

		private:
			void registerKeys();
			void getAvailableFlags();

			bool isPotentialColonialReplacement(const std::pair<std::string, std::shared_ptr<EU4::Country>>& country);
			bool tagIsAlphaDigitDigit(const std::string& tag) const;
			void makeOneMapping(std::shared_ptr<EU4::Country> country, const std::map<std::string, V2Country*>& Vic2Countries);
			std::map<std::string, std::string>::iterator ifValidGetCK2MappingRule(const std::shared_ptr<EU4::Country> country, std::map<std::string, std::string>::iterator mappingRule);
			bool mapToExistingVic2Country(const std::string& possibleVic2Tags, const std::map<std::string, V2Country*>& Vic2Countries, const std::string& EU4Tag);
			bool mapToFirstUnusedVic2Tag(const std::string& possibleVic2Tags, const std::string& EU4Tag);
			std::string generateNewTag();
			void mapToNewTag(const std::string& EU4Tag, const std::string& Vic2Tag);
			bool attemptColonialReplacement(
				std::shared_ptr<EU4::Country> country,
				const EU4::World& srcWorld,
				const std::map<std::string, V2Country*>& Vic2Countries,
				const ProvinceMapper& provinceMapper
			);
			bool capitalInRightEU4Region(const mappers::ColonyStruct& colony, int EU4Capital, const ProvinceMapper& provinceMapper);
			bool capitalInRightVic2Region(
				const mappers::ColonyStruct& colony,
				std::optional<int> Vic2Capital,
				const EU4::World& srcWorld,
				const std::string& EU4Tag,
				const ProvinceMapper& provinceMapper
			);
			bool inCorrectCultureGroup(const mappers::ColonyStruct& colony, const std::string& primaryCulture, const mappers::CultureGroups& cultureGroupsMapper);
			bool tagIsAvailable(const mappers::ColonyStruct& colony, const std::map<std::string, V2Country*>& Vic2Countries);
			void logMapping(const std::string& EU4Tag, const std::string& V2Tag, const std::string& reason);
			bool tagIsAlreadyAssigned(const std::string& Vic2Tag);

			std::map<std::string, std::string> EU4TagToV2TagsRules;
			std::map<std::string, std::string> EU4TagToV2TagMap;
			std::map<std::string, std::string> V2TagToEU4TagMap;

			char generatedV2TagPrefix = 'X';
			int generatedV2TagSuffix = 0;

			std::set<std::string> availableFlags;

			mappers::CK2TitleMapper ck2titleMapper;
			mappers::ColonialTagMapper colonialTagMapper;
	};
}

#endif // COUNTRY_MAPPINGS_H