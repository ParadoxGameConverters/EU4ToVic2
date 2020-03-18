#ifndef COUNTRY_MAPPINGS_H
#define COUNTRY_MAPPINGS_H

#include <map>
#include <optional>
#include <set>
#include "../ColonialTags/ColonialTagsMapper.h"
#include "../ProvinceMappings/ProvinceMapper.h"
#include "../CultureGroups/CultureGroups.h"
#include "../CK2Titles/CK2TitleMapper.h"
#include "../RegionProvinces/RegionProvinceMapper.h"
#include "newParser.h"
#include "CountryMapping.h"

namespace EU4
{
	class Country;
	class World;
}
namespace V2
{
	class Country;
}

namespace mappers
{
	class CountryMappings: commonItems::parser
	{
	public:
		CountryMappings();
		explicit CountryMappings(std::istream& theStream);
	
		[[nodiscard]] std::optional<std::string> getV2Tag(const std::string& eu4Tag) const;
		[[nodiscard]] std::optional<std::string> getCK2Title(const std::string& eu4Tag, const std::string& countryName, const std::set<std::string>& availableFlags) const;
		[[nodiscard]] const auto& getCK2TitleMapper() const { return ck2titleMapper; }
		[[nodiscard]] static bool tagIsAlphaDigitDigit(const std::string& tag);

		void createMappings(const EU4::World& srcWorld, const std::map<std::string, std::shared_ptr<V2::Country>>& vic2Countries, const ProvinceMapper& provinceMapper);

	private:
		void registerKeys();
		void getAvailableFlags();
		void makeOneMapping(const EU4::Country& country, const std::map<std::string, std::shared_ptr<V2::Country>>& vic2Countries);
		void mapToNewTag(const std::string& eu4Tag, const std::string& vic2Tag);

		std::optional<std::string> determineMappableCK2Title(const EU4::Country& country);
		bool attemptStraightMapping(const EU4::Country& country, const std::map<std::string, std::shared_ptr<V2::Country>>& vic2Countries, const std::string& EU4Tag);
		bool mapToExistingVic2Country(const std::string& possibleVic2Tag, const std::map<std::string, std::shared_ptr<V2::Country>>& vic2Countries, const std::string& eu4Tag);
		bool mapToFirstUnusedVic2Tag(const std::string& possibleVic2Tag, const std::string& eu4Tag);
		bool attemptColonialReplacement(
			EU4::Country& country,
			const EU4::World& srcWorld,
			const std::map<std::string, std::shared_ptr<V2::Country>>& vic2Countries,
			const ProvinceMapper& provinceMapper
		);
		static bool capitalInRightEU4Region(const ColonyStruct& colony, int eu4Capital, const ProvinceMapper& provinceMapper);
		static bool inCorrectCultureGroup(const ColonyStruct& colony, const std::string& primaryCulture, const CultureGroups& cultureGroupsMapper);
		static void logMapping(const std::string& eu4Tag, const std::string& v2Tag, const std::string& reason);
		std::string generateNewTag();

		[[nodiscard]] static bool isPotentialColonialReplacement(const std::pair<std::string, std::shared_ptr<EU4::Country>>& country);
		[[nodiscard]] bool capitalInRightVic2Region(
			const ColonyStruct& colony,
			std::optional<int> vic2Capital,
			const EU4::World& srcWorld,
			const std::string& eu4Tag,
			const ProvinceMapper& provinceMapper) const;
		[[nodiscard]] bool tagIsAvailable(const ColonyStruct& colony, const std::map<std::string, std::shared_ptr<V2::Country>>& vic2Countries) const;
		[[nodiscard]] bool tagIsAlreadyAssigned(const std::string& vic2Tag) const;

		std::vector<std::pair<std::string, CountryMapping>> eu4TagToV2TagsRules; // eu4Tag, related_rule
		std::map<std::string, std::string> eu4TagToV2TagMap;
		std::map<std::string, std::string> v2TagToEU4TagMap;
		std::set<std::string> availableFlags;

		char generatedV2TagPrefix = 'X';
		int generatedV2TagSuffix = 0;

		CK2TitleMapper ck2titleMapper;
		ColonialTagMapper colonialTagMapper;
		RegionProvinceMapper regionProvinceMapper;
};
}

#endif // COUNTRY_MAPPINGS_H