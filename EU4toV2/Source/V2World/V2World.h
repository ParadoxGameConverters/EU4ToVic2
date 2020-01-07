#ifndef V2WORLD_H_
#define V2WORLD_H_



#include "V2Country.h"
#include "V2Diplomacy.h"
#include "Country/V2Party.h"
#include "V2Province.h"
#include "Pops/PopTypes.h"
#include "../EU4World/Army/EU4Army.h"
#include "../EU4World/Provinces/EU4Province.h"
#include "../EU4World/Provinces/PopRatio.h"
#include "../Mappers/CultureMapper.h"
#include "../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "../Mappers/ReligionMapper.h"
#include "../Mappers/GovernmentMapper.h"
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
		V2Country* getCountry(std::string tag) const;
		double getDuration() const { return difftime(std::time(0), begin); }

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

		void initializeCultureMappers(const EU4::world& sourceWorld);
		void initializeReligionMapper(const EU4::world& sourceWorld);
		void initializeProvinceMapper();

		void convertCountries(const EU4::world& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper);
		void initializeCountries(const EU4::world& sourceWorld, const mappers::IdeaEffectMapper& ideaEffectMapper);
		V2Country* createOrLocateCountry(const std::string& V2Tag, const std::shared_ptr<EU4::Country> sourceCountry);
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
		void shuffleRgos();
		void convertUncivReforms(const EU4::world& sourceWorld, const mappers::TechGroupsMapper& techGroupsMapper);
		void convertTechs(const EU4::world& sourceWorld);
		void allocateFactories(const EU4::world& sourceWorld);
		void setupPops(const EU4::world& sourceWorld);
		void addUnions();
		void convertArmies(const EU4::world& sourceWorld);

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
		std::unique_ptr<Vic2::TechSchools> techSchools;
		std::map<int, int> leaderIDMap; // <EU4, V2>
		long totalWorldPopulation;
		bool isRandomWorld;
		int techGroupAlgorithm;

		std::unique_ptr<mappers::CultureMapper> cultureMapper;
		std::unique_ptr<mappers::CultureMapper> slaveCultureMapper;
		std::unique_ptr<mappers::ReligionMapper> religionMapper;
		std::unique_ptr<mappers::ProvinceMapper> provinceMapper;

		std::time_t begin = std::time(0);
};



#endif // V2WORLD_H_
