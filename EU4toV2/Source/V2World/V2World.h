#ifndef V2WORLD_H_
#define V2WORLD_H_



#include "V2Country.h"
#include "V2Diplomacy.h"
#include "V2Factory.h"
#include "V2Party.h"
#include "V2Province.h"
#include "../EU4World/EU4Army.h"
#include "../EU4World/Provinces/EU4Province.h"
#include "../EU4World/Provinces/PopRatio.h"
#include "../Mappers/CultureMapper.h"
#include "../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "../Mappers/ReligionMapper.h"
#include "../Mappers/MinorityPopMapper.h"
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
		V2World(const EU4::world& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper, const mappers::TechGroupsMapper& techGroupsMapper);
		V2Province* getProvince(int provNum) const;
		V2Country* getCountry(string tag) const;
		double getDuration() const { return difftime(std::time(0), begin); }

	private:
		void importProvinces();
		set<string> discoverProvinceFilenames();
		void importProvinceClimates();
		void importProvinceLocalizations(const string& file);
		void importProvinceTerrains();
		bool isAProvinceLocalization(const string& line);

		void importDefaultPops();
		void importPopsFromFile(const string& filename, mappers::MinorityPopMapper minorityPopMapper);
		void importPopsFromProvince(shared_ptr<Object> provinceObj, mappers::MinorityPopMapper minorityPopMapper);

		void logPopsByCountry() const;
		void logPopsFromFile(string filename, map<string, map<string, long int>>& popsByCountry) const;
		void logPopsInProvince(shared_ptr<Object> provinceObj, map<string, map<string, long int>>& popsByCountry) const;
		map<string, map<string, long int>>::iterator getCountryForPopLogging(string country, map<string, map<string, long int>>& popsByCountry) const;
		void logPop(shared_ptr<Object> pop, map<string, map<string, long int>>::iterator countryPopItr) const;
		void outputLog(const map<string, map<string, long int>>& popsByCountry) const;

		void findCoastalProvinces();
		void determineIfProvinceIsCoastal(shared_ptr<Object> provinceObj);

		void importPotentialCountries();
		void importPotentialCountry(const string& line, bool dynamicCountry);

		void initializeCultureMappers(const EU4::world& sourceWorld);
		void initializeReligionMapper(const EU4::world& sourceWorld);
		void initializeProvinceMapper();

		void convertCountries(const EU4::world& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper);
		void initializeCountries(const EU4::world& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper);
		V2Country* createOrLocateCountry(const string& V2Tag, const shared_ptr<EU4::Country> sourceCountry);
		void convertNationalValues(const mappers::IdeaEffectMapper& ideaEffectMapper);
		void convertPrestige();
		void addAllPotentialCountries();
		unsigned int countCivilizedNations();

		void convertProvinces(const EU4::world& sourceWorld);
		std::vector<V2Demographic> determineDemographics(
			const EU4::Regions& eu4Regions,
			std::vector<EU4::PopRatio>& popRatios,
			const EU4::Province* eProv,
			V2Province* vProv,
			std::string oldOwnerTag,
			int destNum,
			double provPopRatio
		);

		void convertDiplomacy(const EU4::world& sourceWorld);
		void setupColonies();
		void setupStates();
		void convertUncivReforms(const EU4::world& sourceWorld, const mappers::TechGroupsMapper& techGroupsMapper);
		void convertTechs(const EU4::world& sourceWorld);
		void allocateFactories(const EU4::world& sourceWorld);
		void setupPops(const EU4::world& sourceWorld);
		void addUnions();
		void convertArmies(const EU4::world& sourceWorld);

		void output(unsigned int potentialGPs) const;
		void createModFile() const;
		void outputPops() const;

		map<int, V2Province*> provinces;
		map<string, V2Country*> countries;
		map<string, V2Country*> potentialCountries;
		map<string, V2Country*> dynamicCountries;
		V2Diplomacy diplomacy;
		map<int, set<string>> colonies;
		map<string, list<int>>	popRegions;
		std::unique_ptr<Vic2::TechSchools> techSchools;
		map<int, int> leaderIDMap; // <EU4, V2>
		long totalWorldPopulation;
		bool isRandomWorld;
		int	techGroupAlgorithm;

		std::unique_ptr<mappers::CultureMapper> cultureMapper;
		std::unique_ptr<mappers::CultureMapper> slaveCultureMapper;
		std::unique_ptr<mappers::ReligionMapper> religionMapper;
		std::unique_ptr<mappers::ProvinceMapper> provinceMapper;

		std::time_t begin = std::time(0);
};



#endif // V2WORLD_H_
