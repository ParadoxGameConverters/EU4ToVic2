#ifndef WORLD_H
#define WORLD_H

#include "Country/Country.h"
#include "Country/CountryPopLogger.h"
#include "Diplomacy/Diplomacy.h"
#include "Province/Province.h"
#include "../EU4World/Provinces/EU4Province.h"
#include "../EU4World/Provinces/PopRatio.h"
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
#include "../Mappers/Pops/PopTypes.h"
#include "../Mappers/Unreleasables/Unreleasables.h"
#include "../Mappers/LeaderTraits/LeaderTraitMapper.h"
#include "../Mappers/NavalBases/NavalBaseMapper.h"
#include "../Mappers/RGORandomization/BucketList.h"
#include "../Mappers/PortProvinces/PortProvinces.h"
#include "Province/ProvinceNameParser.h"
#include <list>
#include <memory>
#include <set>
#include <time.h>
#include "MappingChecker/MappingChecker.h"

namespace mappers {
	class TechGroupsMapper;
}

namespace V2
{
	class World
	{
	public:
		World(const EU4::World& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper, const mappers::TechGroupsMapper& techGroupsMapper);

		
	private:
		long totalWorldPopulation = 0;

		std::map<std::string, std::list<int>> popRegions;
		std::map<int, std::shared_ptr<Province>> provinces;
		std::map<std::string, std::shared_ptr<Country>> countries;
		std::map<std::string, std::shared_ptr<Country>> potentialCountries;
		std::map<std::string, std::shared_ptr<Country>> dynamicCountries;


		void importProvinces();
		std::set<std::string> discoverProvinceFilenames();
		void shuffleRgos();
		void importDefaultPops();
		void importPopsFromFile(const std::string& filename, const mappers::MinorityPopMapper& minorityPopMapper);
		void importPopsFromProvince(int provinceID, const mappers::PopTypes& popType, const mappers::MinorityPopMapper& minorityPopMapper);
		void importPotentialCountries();
		void importPotentialCountry(const std::string& line, bool dynamicCountry);
		void initializeCultureMappers(const EU4::World& sourceWorld);
		void convertCountries(const EU4::World& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper);
		void convertProvinces(const EU4::World& sourceWorld);
		std::optional<std::string> determineProvinceOwnership(const std::vector<int>& eu4ProvinceNumbers, const EU4::World& sourceWorld) const;
		void initializeCountries(const EU4::World& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper);

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
		ProvinceNameParser provinceNameParser;
		CountryPopLogger countryPopLogger;
		MappingChecker mappingChecker;
		
		/*
	
		std::shared_ptr<Province> getProvince(int provNum) const;
		std::shared_ptr<Country> getCountry(std::string tag) const;
		const mappers::PartyNameMapper& getPartyNameMapper() const { return partyNameMapper; }
		const mappers::PartyTypeMapper& getPartyTypeMapper() const { return partyTypeMapper; }

		bool isAProvinceLocalization(const std::string& line);






		std::shared_ptr<Country> createOrLocateCountry(const std::string& V2Tag, std::shared_ptr<EU4::Country> sourceCountry);
		void convertNationalValues(const mappers::IdeaEffectMapper& ideaEffectMapper);
		void convertPrestige();
		void addAllPotentialCountries();
		unsigned int countCivilizedNations();

		std::vector<V2::Demographic> determineDemographics(
			const EU4::Regions& eu4Regions,
			std::vector<EU4::PopRatio>& popRatios,
			const EU4::Province* eProv,
			std::shared_ptr<V2::Province> vProv,
			std::string oldOwnerTag,
			int destNum,
			double provPopRatio
		);

		void setupColonies();
		void setupStates();
		void convertUncivReforms(const EU4::World& sourceWorld, const mappers::TechGroupsMapper& techGroupsMapper);
		void convertTechs(const EU4::World& sourceWorld);
		void allocateFactories(const EU4::World& sourceWorld);
		void setupPops(const EU4::World& sourceWorld);
		void addUnions();
		void convertArmies(const EU4::World& sourceWorld);

		void output(unsigned int potentialGPs) const;
		void createModFile() const;
		void outputPops() const;

		Diplomacy diplomacy;
		std::map<int, std::set<std::string>> colonies;
		std::map<int, int> leaderIDMap; // <EU4, V2>
		bool isRandomWorld = false;
		int techGroupAlgorithm = 0;
		int stateId = 0; // ID counter for generated states

		*/
	};
}
#endif // WORLD_H
