#ifndef EU4_WORLD_H
#define EU4_WORLD_H
#include "Buildings/Buildings.h"
#include "ConverterVersion.h"
#include "Country/EU4Country.h"
#include "CultureGroups/CultureGroupsManager.h"
#include "Diplomacy/EU4Diplomacy.h"
#include "GameVersion.h"
#include "IdeaEffects/IdeaEffectMapper.h"
#include "Localization/EU4Localization.h"
#include "Parser.h"
#include "Provinces/Provinces.h"
#include "Regions/Regions.h"
#include "Religions/Religions.h"
#include "SuperGroupMapper/SuperGroupMapper.h"
#include "UnitTypes/UnitTypeMapper.h"
#include "Wars/EU4War.h"
#include <map>
#include <memory>

namespace EU4
{
class World: commonItems::parser
{
  public:
	World(const mappers::IdeaEffectMapper& ideaEffectMapper, const commonItems::ConverterVersion& converterVersion);

	[[nodiscard]] std::shared_ptr<Province> getProvince(int provNum) const { return provinces->getProvince(provNum); }
	[[nodiscard]] std::shared_ptr<Country> getCountry(const std::string& tag) const;

	[[nodiscard]] const auto& getCountries() const { return theCountries; }
	[[nodiscard]] const auto& getDiplomaticAgreements() const { return diplomacy; }
	[[nodiscard]] const auto& getRegions() const { return *regions; }
	[[nodiscard]] const auto& getReligions() const { return theReligions; }
	[[nodiscard]] const auto& getVersion() const { return *version; }
	[[nodiscard]] const auto& getWars() const { return wars; }
	[[nodiscard]] const auto& getCultureGroupsMapper() const { return cultureGroupsMapper; }
	[[nodiscard]] const auto& getProvinces() const { return provinces->getAllProvinces(); }
	[[nodiscard]] const auto& getHistoricalData() const { return historicalData; }
	[[nodiscard]] const auto& getNativeCultures() const { return nativeCultures; }
	[[nodiscard]] const auto& getHREReforms() const { return hreReforms; }
	[[nodiscard]] bool decentralizedHRE() const { return hreReforms.contains("emperor_reichskrieg"); }
	[[nodiscard]] const auto& getShatteredHreTag() const { return shatteredHreTag; }
	[[nodiscard]] const auto& getEU4Localization() const { return EU4localization; }

  private:
	void registerKeys(const mappers::IdeaEffectMapper& ideaEffectMapper, const commonItems::ConverterVersion& converterVersion);

	void verifySave();
	void loadRevolutionTarget();
	void addProvinceInfoToCountries();
	void loadRegions();
	void loadEU4RegionsNewVersion();
	void loadEU4RegionsOldVersion();
	void readCommonCountries();
	void readCommonCountriesFile(std::istream& in, const std::filesystem::path& rootPath);
	void setLocalizations();
	void resolveRegimentTypes();
	void mergeNations();
	void uniteJapan();
	void removeEmptyNations();
	void removeDeadLandlessNations();
	void removeLandlessNations();
	void setEmpires();
	void assignProvincesToAreas(const std::map<std::string, std::set<int>>& theAreas) const;
	void fillHistoricalData();
	void buildProvinceWeights() const;
	void catalogueNativeCultures();
	void generateNeoCultures();
	void buildPopRatios() const;
	void calculateIndustry() const;
	std::string generateNeoCulture(const std::string& superRegionName, const std::string& oldCultureName);
	void markNewWorldCountries();
	void importShatteredHreTag();

	struct saveData
	{
		bool parsedMeta = false;
		std::string metadata;
		std::string gamestate;
	};
	saveData saveGame;

	std::string holyRomanEmperor;
	std::optional<std::string> shatteredHreTag;
	std::set<std::string> hreReforms;
	std::string celestialEmperor;
	std::unique_ptr<Regions> regions;
	std::unique_ptr<Provinces> provinces;
	std::map<std::string, std::shared_ptr<Country>> theCountries;
	std::vector<EU4Agreement> diplomacy;
	std::unique_ptr<GameVersion> version;
	std::string revolutionTargetString;
	std::map<std::string, mappers::UnitType> unitTypeMap;
	std::vector<War> wars;
	std::map<std::string, std::set<std::string>> nativeCultures;						// superregion-culturenames
	std::map<std::pair<std::string, std::string>, std::string> generatedCultures; // origculture/superregion - neoculture (cache)

	Religions theReligions;
	mappers::UnitTypeMapper unitTypeMapper;
	std::unique_ptr<mappers::Buildings> buildingTypes;
	std::shared_ptr<mappers::CultureGroupsManager> cultureGroupsMapper;
	mappers::SuperGroupMapper superGroupMapper;
	EU4Localization EU4localization;

	// export data for hoi4
	std::vector<std::pair<std::string, HistoricalEntry>> historicalData;

	parser revolutionParser;
};
} // namespace EU4

#endif // EU4_WORLD_H
