#ifndef COUNTRY_MAPPINGS_H
#define COUNTRY_MAPPINGS_H
#include <map>
#include <optional>
#include <set>
#include "../ColonialTags/ColonialTagsMapper.h"
#include "../ProvinceMappings/ProvinceMapper.h"
#include "../CultureGroups/CultureGroups.h"
#include "../Titles/TitleMapper.h"
#include "V2ProvinceRegionsMapper/V2ProvinceRegionsMapper.h"
#include "Parser.h"
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
		[[nodiscard]] std::optional<std::string> getTitle(const std::string& eu4Tag,
			 const std::string& countryName,
			 const std::set<std::string>& availableFlags) const;
		[[nodiscard]] const auto& getTitleMapper() const { return titleMapper; }
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
		std::string generateNewTag();

		[[nodiscard]] static bool isPotentialColonialReplacement(const std::pair<std::string, std::shared_ptr<EU4::Country>>& country);
		[[nodiscard]] bool tagIsAvailable(const std::string& colonyTag, const std::map<std::string, std::shared_ptr<V2::Country>>& vic2Countries) const;
		[[nodiscard]] bool tagIsAlreadyAssigned(const std::string& vic2Tag) const;

		std::vector<std::pair<std::string, CountryMapping>> eu4TagToV2TagsRules; // eu4Tag, related_rule
		std::map<std::string, std::string> eu4TagToV2TagMap;
		std::map<std::string, std::string> v2TagToEU4TagMap;
		std::set<std::string> availableFlags;

		char generatedV2TagPrefix = 'X';
		int generatedV2TagSuffix = 0;

		TitleMapper titleMapper;
		ColonialTagMapper colonialTagMapper;
		V2ProvinceRegionsMapper v2provinceRegionsMapper;
};
}

#endif // COUNTRY_MAPPINGS_H