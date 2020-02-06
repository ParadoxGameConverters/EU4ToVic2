#ifndef WORLD_H
#define WORLD_H

#include "Country/Country.h"
#include "../EU4World/Country/EU4Country.h"
#include "Country/CountryPopLogger.h"
#include "Diplomacy/Diplomacy.h"
#include "Province/Province.h"
#include "../Mappers/CultureMapper/CultureMapper.h"
#include "../Mappers/Geography/Continents.h"
#include "../Mappers/Geography/ClimateMapper.h"
#include "../Mappers/Geography/TerrainDataMapper.h"
#include "../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "../Mappers/ReligionMapper/ReligionMapper.h"
#include "../Mappers/GovernmentMapper/GovernmentMapper.h"
#include "../Mappers/MinorityPops/MinorityPopMapper.h"
#include "../Mappers/CountryMappings/CountryMappings.h"
#include "../Mappers/Adjacency/AdjacencyMapper.h"
#include "../Mappers/PartyNames/PartyNameMapper.h"
#include "../Mappers/PartyTypes/PartyTypeMapper.h"
#include "../Mappers/RegimentCosts/RegimentCostsMapper.h"
#include "../Mappers/StateMapper/StateMapper.h"
#include "../Mappers/TechSchools/TechSchoolMapper.h"
#include "../Mappers/CulturalUnions/CulturalUnionMapper.h"
#include "../Mappers/FactoryTypes/FactoryTypeMapper.h"
#include "../Mappers/Unreleasables/Unreleasables.h"
#include "../Mappers/LeaderTraits/LeaderTraitMapper.h"
#include "../Mappers/NavalBases/NavalBaseMapper.h"
#include "../Mappers/RGORandomization/BucketList.h"
#include "../Mappers/PortProvinces/PortProvinces.h"
#include "../Mappers/WarGoalMapper/WarGoalMapper.h"
#include "../Mappers/StartingTechMapper/StartingTechMapper.h"
#include "../Mappers/StartingInventionMapper/StartingInventionMapper.h"
#include "../Mappers/CultureGroups/CultureGroups.h"
#include "../Mappers/RegionLocalizations/RegionLocalizations.h"
#include "Province/ProvinceNameParser.h"
#include <list>
#include <memory>
#include <set>
#include "MappingChecker/MappingChecker.h"
#include "Output/ModFile.h"
#include "War/War.h"

namespace mappers {
	class VersionParser;
	class TechGroupsMapper;
}

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
		bool isRandomWorld = false;

		std::map<std::string, std::list<int>> popRegions;
		std::map<int, std::shared_ptr<Province>> provinces;
		std::map<std::string, std::shared_ptr<Country>> countries;
		std::map<std::string, std::shared_ptr<Country>> potentialCountries;
		std::map<std::string, std::shared_ptr<Country>> dynamicCountries;
		std::vector<War> wars;
		std::vector<std::pair<std::string, EU4::HistoricalEntry>> historicalData; // HoI4 export dynasty+rulers
		std::set<std::string> neoCultureLocalizations; // raw strings for output.

		[[nodiscard]] std::optional<std::string> determineProvinceOwnership(const std::vector<int>& eu4ProvinceNumbers, const EU4::World& sourceWorld) const;
		[[nodiscard]] std::shared_ptr<Province> getProvince(int provID) const;
		[[nodiscard]] std::shared_ptr<Country> getCountry(const std::string& tag) const;
		[[nodiscard]] unsigned int countCivilizedNations() const;

		static std::optional<std::string> determineProvinceControllership(const std::vector<int>& eu4ProvinceNumbers, const EU4::World& sourceWorld);
		std::shared_ptr<Country> createOrLocateCountry(const std::string& V2Tag, const EU4::Country& sourceCountry);
		static std::set<std::string> discoverProvinceFilenames();

		void importProvinces();
		void shuffleRgos();
		void importDefaultPops();
		void importPopsFromFile(const std::string& filename, const mappers::MinorityPopMapper& minorityPopMapper);
		void importPopsFromProvince(int provinceID, const mappers::PopTypes& popType, const mappers::MinorityPopMapper& minorityPopMapper);
		void importPotentialCountries();
		void importPotentialCountry(const std::string& line, bool dynamicCountry);
		void initializeCultureMappers();
		void convertCountries(const EU4::World& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper);
		void convertProvinces(const EU4::World& sourceWorld);
		void initializeCountries(const EU4::World& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper);
		void convertNationalValues();
		void convertPrestige();
		void addAllPotentialCountries();
		void setupColonies();
		void setupStates();
		void convertUncivReforms(const EU4::World& sourceWorld, const mappers::TechGroupsMapper& techGroupsMapper);
		void convertTechs(const EU4::World& sourceWorld);
		void allocateFactories(const EU4::World& sourceWorld);
		void setupPops(const EU4::World& sourceWorld);
		void addUnions();
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
		
		mappers::ProvinceMapper provinceMapper;
		mappers::Continents continentsMapper;
		mappers::CountryMappings countryMapper;
		mappers::AdjacencyMapper adjacencyMapper;
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
		mappers::CulturalUnionMapper culturalUnionMapper;
		mappers::CulturalUnionMapper culturalNationalitiesMapper;
		mappers::FactoryTypeMapper factoryTypeMapper;
		mappers::Unreleasables unreleasablesMapper;
		mappers::LeaderTraitMapper leaderTraitMapper;
		mappers::NavalBaseMapper navalBaseMapper;
		mappers::BucketList bucketShuffler;
		mappers::PortProvinces portProvincesMapper;
		mappers::WarGoalMapper warGoalMapper;
		mappers::StartingTechMapper startingTechMapper;
		mappers::StartingInventionMapper startingInventionMapper;
		mappers::CultureGroups cultureGroupsMapper;
		mappers::RegionLocalizations regionLocalizations;
		ProvinceNameParser provinceNameParser;
		CountryPopLogger countryPopLogger;
		MappingChecker mappingChecker;
		ModFile modFile;
		Diplomacy diplomacy;
	};
	
	std::ostream& operator<<(std::ostream& output, const std::vector<std::pair<std::string, EU4::HistoricalEntry>>& historicalData);
}

#endif // WORLD_H
