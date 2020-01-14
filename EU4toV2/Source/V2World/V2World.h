#ifndef V2WORLD_H_
#define V2WORLD_H_



#include "V2Country.h"
#include "V2Diplomacy.h"
#include "V2Province.h"
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
#include "../Mappers/RegimentCosts/RegimentCostsMapper.h"
#include "../Mappers/StateMapper/StateMapper.h"
#include "../Mappers/TechSchools/TechSchoolMapper.h"
#include "../Mappers/CulturalUnions/CulturalUnionMapper.h"
#include "../Mappers/FactoryTypes/FactoryTypeMapper.h"
#include "../Mappers/Pops/PopTypes.h"
#include "../Mappers/Unreleasables/Unreleasables.h"
#include <list>
#include <memory>
#include <set>
#include <time.h>



class V2Country;
class V2Army;
class V2LeaderTraits;



class V2World
{
	public:
		V2World(const EU4::World& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper, const mappers::TechGroupsMapper& techGroupsMapper);
		V2Province* getProvince(int provNum) const;
		V2Country* getCountry(std::string tag) const;
		double getDuration() const { return difftime(std::time(0), begin); }
		const mappers::PartyNameMapper& getPartyNameMapper() const { return partyNameMapper; }

	private:
		void importProvinces();
		std::set<std::string> discoverProvinceFilenames();
		void importProvinceClimates();
		void importProvinceLocalizations(const std::string& file);
		void importProvinceTerrains();
		bool isAProvinceLocalization(const std::string& line);

		void importDefaultPops();
		void importPopsFromFile(const std::string& filename, const mappers::MinorityPopMapper& minorityPopMapper);
		void importPopsFromProvince(const int provinceID, const mappers::PopTypes& popType, const mappers::MinorityPopMapper& minorityPopMapper);

		void logPopsByCountry() const;
		void logPopsFromFile(std::string filename, std::map<std::string, std::map<std::string, long int>>& popsByCountry) const;
		void logPopsInProvince(const int& provinceID, const mappers::PopTypes& popType, std::map<std::string, std::map<std::string, long int>>& popsByCountry) const;
		std::map<std::string, std::map<std::string, long int>>::iterator getCountryForPopLogging(std::string country, std::map<std::string, std::map<std::string, long int>>& popsByCountry) const;
		void logPop(const std::string& popType, const mappers::Pop& pop, std::map<std::string, std::map<std::string, long int>>::iterator countryPopItr) const;
		void outputLog(const std::map<std::string, std::map<std::string, long int>>& popsByCountry) const;

		void findCoastalProvinces();

		void importPotentialCountries();
		void importPotentialCountry(const std::string& line, bool dynamicCountry);

		void initializeCultureMappers(const EU4::World& sourceWorld);

		void convertCountries(const EU4::World& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper);
		void initializeCountries(const EU4::World& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper);
		V2Country* createOrLocateCountry(const std::string& V2Tag, const std::shared_ptr<EU4::Country> sourceCountry);
		void convertNationalValues(const mappers::IdeaEffectMapper& ideaEffectMapper);
		void convertPrestige();
		void addAllPotentialCountries();
		unsigned int countCivilizedNations();

		void convertProvinces(const EU4::World& sourceWorld);
		std::vector<V2Demographic> determineDemographics(
			const EU4::Regions& eu4Regions,
			std::vector<EU4::PopRatio>& popRatios,
			const EU4::Province* eProv,
			V2Province* vProv,
			std::string oldOwnerTag,
			int destNum,
			double provPopRatio
		);

		void convertDiplomacy(const EU4::World& sourceWorld);
		void setupColonies();
		void setupStates();
		void shuffleRgos();
		void convertUncivReforms(const EU4::World& sourceWorld, const mappers::TechGroupsMapper& techGroupsMapper);
		void convertTechs(const EU4::World& sourceWorld);
		void allocateFactories(const EU4::World& sourceWorld);
		void setupPops(const EU4::World& sourceWorld);
		void addUnions();
		void convertArmies(const EU4::World& sourceWorld);

		void output(unsigned int potentialGPs) const;
		void createModFile() const;
		void outputPops() const;

		std::map<int, V2Province*> provinces;
		std::map<std::string, V2Country*> countries;
		std::map<std::string, V2Country*> potentialCountries;
		std::map<std::string, V2Country*> dynamicCountries;
		V2Diplomacy diplomacy;
		std::map<int, std::set<std::string>> colonies;
		std::map<std::string, std::list<int>>	popRegions;
		std::map<int, int> leaderIDMap; // <EU4, V2>
		long totalWorldPopulation;
		bool isRandomWorld;
		int techGroupAlgorithm;

		std::time_t begin = std::time(0);

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
		mappers::RegimentCostsMapper regimentCostsMapper;
		mappers::ReligionMapper religionMapper;
		mappers::StateMapper stateMapper;
		mappers::TechSchoolMapper techSchoolMapper;
		mappers::CulturalUnionMapper culturalUnionMapper;
		mappers::CulturalUnionMapper culturalNationalitiesMapper;
		mappers::FactoryTypeMapper factoryTypeMapper;
		mappers::Unreleasables unreleasablesMapper;
};



#endif // V2WORLD_H_
