#ifndef WORLD_H
#define WORLD_H
#include "../EU4World/Country/EU4Country.h"
#include "../Mappers/AcceptedCultureThresholds/AcceptedCultureThresholdsMapper.h"
#include "../Mappers/AfricaReset/AfricaResetMapper.h"
#include "../Mappers/CountryFlags/CountryFlags.h"
#include "../Mappers/CountryMappings/CountryMappings.h"
#include "../Mappers/CulturalUnions/CulturalUnionMapper.h"
#include "../Mappers/CultureGroups/CultureGroups.h"
#include "../Mappers/CultureMapper/CultureMapper.h"
#include "../Mappers/DeadDefinitionMapper/DeadDefinitionMapper.h"
#include "../Mappers/FactoryTypes/FactoryTypeMapper.h"
#include "../Mappers/Geography/ClimateMapper.h"
#include "../Mappers/Geography/Continents.h"
#include "../Mappers/Geography/TerrainDataMapper.h"
#include "../Mappers/GovernmentMapper/GovernmentMapper.h"
#include "../Mappers/Issues/Issues.h"
#include "../Mappers/Issues/ModReforms.h"
#include "../Mappers/LeaderTraits/LeaderTraitMapper.h"
#include "../Mappers/MinorityPops/MinorityPopMapper.h"
#include "../Mappers/NavalBases/NavalBaseMapper.h"
#include "../Mappers/PartyNames/PartyNameMapper.h"
#include "../Mappers/PartyTypes/PartyTypeMapper.h"
#include "../Mappers/PortProvinces/PortProvinces.h"
#include "../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "../Mappers/RGORandomization/BucketList.h"
#include "../Mappers/RegimentCosts/RegimentCostsMapper.h"
#include "../Mappers/RegionLocalizations/RegionLocalizations.h"
#include "../Mappers/ReligionMapper/ReligionMapper.h"
#include "../Mappers/StartingInventionMapper/StartingInventionMapper.h"
#include "../Mappers/StateMapper/StateMapper.h"
#include "../Mappers/TechSchools/TechSchoolMapper.h"
#include "../Mappers/Technologies/Technologies.h"
#include "../Mappers/Unreleasables/Unreleasables.h"
#include "../Mappers/WarGoalMapper/WarGoalMapper.h"
#include "Country/Country.h"
#include "Diplomacy/Diplomacy.h"
#include "Events/Events.h"
#include "MappingChecker/MappingChecker.h"
#include "Output/ModFile.h"
#include "Province/Province.h"
#include "Province/ProvinceNameParser.h"
#include "UnionTagsMapper/UnionTagsMapper.h"
#include "VanillaWorld/VanillaWorld.h"
#include "War/War.h"
#include <list>
#include <memory>
#include <set>

namespace mappers
{
class VersionParser;
class TechGroupsMapper;
} // namespace mappers

namespace V2
{
class World
{
  public:
	World(const EU4::World& sourceWorld,
		 const mappers::IdeaEffectMapper& ideaEffectMapper,
		 const mappers::TechGroupsMapper& techGroupsMapper,
		 const mappers::VersionParser& versionParser);

  private:
	long totalWorldPopulation = 0;
	int stateId = 0; // ID counter for generated states

	std::map<std::string, std::list<int>> popRegions;
	std::map<int, std::shared_ptr<Province>> provinces;
	std::map<std::string, std::shared_ptr<Country>> countries;
	std::map<std::string, std::shared_ptr<Country>> potentialCountries;
	std::map<std::string, std::shared_ptr<Country>> dynamicCountries;
	std::vector<War> wars;
	std::vector<std::pair<std::string, EU4::HistoricalEntry>> historicalData; // HoI4 export dynasty+rulers
	std::set<std::string> neoCultureLocalizations;									  // raw strings for output.
	std::map<int, int> stateMap;
	std::map<int, int> provinceMap;
	std::vector<std::string> reassignedTags;

	[[nodiscard]] std::optional<std::string> determineProvinceOwnership(const std::set<int>& eu4ProvinceNumbers, const EU4::World& sourceWorld) const;
	[[nodiscard]] std::shared_ptr<Province> getProvince(int provID) const;
	[[nodiscard]] std::shared_ptr<Country> getCountry(const std::string& tag) const;
	[[nodiscard]] unsigned int countCivilizedNations() const;
	[[nodiscard]] std::shared_ptr<Country> getHreEmperor() const;
	[[nodiscard]] std::string clipCountryFileName(const std::string& incoming) const;
	[[nodiscard]] const auto& getStateMap() const { return stateMap; }
	[[nodiscard]] const auto& getProvinceMap() const { return provinceMap; }
	[[nodiscard]] const auto& getModReforms() const { return modReforms; }

	static std::optional<std::string> determineProvinceControllership(const std::set<int>& eu4ProvinceNumbers, const EU4::World& sourceWorld);
	std::shared_ptr<Country> createOrLocateCountry(const std::string& V2Tag, const EU4::Country& sourceCountry);
	static std::set<std::string> discoverProvinceFilenames();
	static std::set<std::string> discoverModProvinceFilenames();

	void dropStates(const mappers::TechGroupsMapper& techGroupsMapper);
	void dropCores();
	void importProvinces();
	void importModProvinces();
	void shuffleRgos();
	void importDefaultPops();
	void importPopsFromFile(const std::string& filename);
	void importPopsFromProvince(int provinceID, const std::vector<mappers::PopDetails>& popsDetails);
	void importPotentialCountries();
	void importPotentialCountry(const std::string& line, bool dynamicCountry);
	void initializeCultureMappers();
	void convertCountries(const EU4::World& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper);
	void convertProvinces(const EU4::World& sourceWorld, const mappers::TechGroupsMapper& techGroupsMapper, const EU4::Regions& eu4Regions);
	void initializeCountries(const EU4::World& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper);
	void convertNationalValues();
	void convertPrestige();
	void addAllPotentialCountries();
	void setupStates();
	void convertUncivReforms(const EU4::World& sourceWorld, const mappers::TechGroupsMapper& techGroupsMapper);
	void convertTechs(const EU4::World& sourceWorld);
	void allocateFactories(const EU4::World& sourceWorld);
	void setupPops(const EU4::World& sourceWorld);
	void addUnions(bool hreDecentralized, const std::shared_ptr<Country>& emperor);
	void decentralizeHRE(bool hreDecentralized, const std::shared_ptr<Country>& emperor);
	void convertArmies();
	void output(const mappers::VersionParser& versionParser) const;
	void createModFile() const;
	void outputPops() const;
	static void outputVersion(const mappers::VersionParser& versionParser);
	void modifyDefines() const;
	void outputCommonCountries() const;
	void outputLocalisation() const;
	void outputProvinces() const;
	void outputCountries() const;
	void outputWars() const;
	void outputHistory() const;
	void outputCultures() const;
	void outputNeoCultures() const;
	void verifyCountriesWritten() const;
	void convertWars(const EU4::World& sourceWorld);
	void transcribeHistoricalData();
	void transcribeNeoCultures();
	void modifyPrimaryAndAcceptedCultures();
	void addAcceptedCultures(const EU4::Regions& eu4Regions);
	void addReligionCulture();
	void convertCountryFlags();
	void updateDeadNations();

	void addStateMapping(int origID, int modID) { stateMap.insert(std::make_pair(origID, modID)); }
	int findBestMatch(const std::pair<int, std::map<int, int>>& state);
	void sortStateMap(std::map<int, std::map<int, int>>& freqMap, std::vector<int>& sortedStates);
	void updateStateMap(std::map<int, std::map<int, int>>& freqMap, int modStateID);
	void addProvinceMapping(int origID, int modID) { provinceMap.insert(std::make_pair(origID, modID)); }
	void copyFolder(const std::string& folder, const std::string& src, const std::string& dest) const;
	void mergeFolder(const std::string& folder, const std::string& src, const std::string& dest) const;
	void copyModFiles() const;
	void outputStateMap(std::string srcFile, std::string outFile) const;
	void outStateMap(std::string outFile) const;
	void outProvinceMap(std::string outFile) const;
	void outEvents() const;
	void convertEvents();
	void drawProvinceMap();
	void drawStateMap();
	bool verifyMap(std::map<int, int> theMap) const;
	void mapUnlocalized(const std::vector<int>& vanillaProvs,
	 std::map<int, std::string> locProvs,
	 std::ofstream& output);
	void mapUnchanged(const std::vector<int>& vanillaProvs,
	 const mappers::ProvinceMapper& vanillaMapper,
	 const mappers::ProvinceMapper& modMapper,
	 std::ofstream& output);
	void mapLeftovers(const std::vector<int>& vanillaProvs,
	 const mappers::ProvinceMapper& vanillaMapper,
	 const mappers::ProvinceMapper& modMapper,
	 std::ofstream& output);
	void updateCountryDetails();
	void importUnitColors();
	std::vector<std::string> getIssues(const std::string& issueCategory);
	void identifyReassignedTags();
	bool isTagReassigned(const std::string& tag) const;
	void updateFlags() const;
	void outputGTFO(std::map<std::string, std::shared_ptr<Country>> countries) const;
	void outputReturnCores(std::map<std::string, std::shared_ptr<V2::Country>> countries) const;
	void outputTechnologies() const;
	void outputUnitColors() const;
	int getModStateId(int province, std::ofstream& output);

	mappers::ProvinceMapper provinceMapper;
	mappers::Continents continentsMapper;
	mappers::CountryMappings countryMapper;
	mappers::ClimateMapper climateMapper;
	mappers::TerrainDataMapper terrainDataMapper;
	mappers::CultureMapper cultureMapper;
	mappers::CultureMapper slaveCultureMapper;
	mappers::GovernmentMapper governmentMapper;
	mappers::MinorityPopMapper minorityPopMapper;
	mappers::PartyNameMapper partyNameMapper;
	mappers::PartyTypeMapper partyTypeMapper;
	mappers::RegimentCostsMapper regimentCostsMapper;
	mappers::ReligionMapper religionMapper;
	mappers::StateMapper stateMapper;
	mappers::TechSchoolMapper techSchoolMapper;
	std::unique_ptr<mappers::CulturalUnionMapper> culturalUnionMapper;
	std::unique_ptr<mappers::CulturalUnionMapper> culturalNationalitiesMapper;
	mappers::FactoryTypeMapper factoryTypeMapper;
	mappers::Unreleasables unreleasablesMapper;
	mappers::LeaderTraitMapper leaderTraitMapper;
	mappers::NavalBaseMapper navalBaseMapper;
	mappers::BucketList bucketShuffler;
	mappers::PortProvinces portProvincesMapper;
	mappers::WarGoalMapper warGoalMapper;
	mappers::StartingInventionMapper startingInventionMapper;
	mappers::CultureGroups cultureGroupsMapper;
	mappers::RegionLocalizations regionLocalizations;
	mappers::AfricaResetMapper africaResetMapper;
	mappers::AcceptedCultureThresholdsMapper acceptedCultureThresholdsMapper;
	mappers::DeadDefinitionMapper deadDefinitionMapper;
	ProvinceNameParser provinceNameParser;
	MappingChecker mappingChecker;
	ModFile modFile;
	Diplomacy diplomacy;
	mappers::CountryFlags countryFlags;
	mappers::UnionTagsMapper unionTagsMapper;
	std::map<std::string, std::string> events;
	VanillaWorld vanillaWorld;
	mappers::Issues issues;
	mappers::ModReforms modReforms;
	mappers::Technologies technologies;
};

std::ostream& operator<<(std::ostream& output, const std::vector<std::pair<std::string, EU4::HistoricalEntry>>& historicalData);
} // namespace V2

#endif // WORLD_H
