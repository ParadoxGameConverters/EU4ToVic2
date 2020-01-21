#ifndef PROVINCE_H
#define PROVINCE_H

#include <string>
#include "../../Mappers/ProvinceDetails/ProvinceDetails.h"
#include "../../Mappers/Geography/ClimateMapper.h"
#include "../../Mappers/Geography/TerrainDataMapper.h"
#include "../Pop/Pop.h"
#include "../../EU4World/Provinces/EU4Province.h"
#include "../../EU4World/Country/EU4Country.h"
#include "../Factory/Factory.h"
#include "../../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "ProvinceNameParser.h"
#include "../../Mappers/NavalBases/NavalBaseMapper.h"

namespace EU4 {
	class Regions;
}

namespace V2
{
	enum class REGIMENTTYPE;
	class Country;
	
	struct Demographic
	{
		std::string culture;
		std::string slaveCulture;
		std::string religion;
		double upperRatio = 0.0;
		double middleRatio = 0.0;
		double lowerRatio = 0.0;
	};

	class Province
	{
	public:
		Province(
			const std::string& _filename, 
			const mappers::ClimateMapper& climateMapper, 
			const mappers::TerrainDataMapper& terrainDataMapper,
			const ProvinceNameParser& provinceNameParser,
			mappers::NavalBaseMapper navalBaseMapper);
		int getID() const { return provinceID; }
		std::string getRgoType() const { return details.rgoType; }
		std::string getClimate() const { return details.climate; }
		std::string getTerrain() const { return details.terrain; }
		void setRgoType(const std::string& _type) { details.rgoType = _type; }
		void setOwner(std::string _owner) { details.owner = _owner; }
		void setController(std::string _controller) { details.controller = _controller; }
		std::string getName() const { return name; }
		std::string getOwner() const { return details.owner; }
		std::string getController() const { return details.controller; }
		bool isCoastal() const { return coastal; }
		void setResettable() { resettable = true; }
		bool getResettable() const { return resettable; }
		void clearCores() { details.cores.clear(); }
		void addCore(std::string);
		bool isTerritorialCore() const { return territorialCore; }
		void addVanillaPop(std::shared_ptr<Pop> vanillaPop);
		void addMinorityPop(std::shared_ptr<Pop> minorityPop);
		void setSlaveProportion(const double _pro) { slaveProportion = _pro; }
		void increaseRailLevel(int level) { railLevel += level; }
		void setFortLevel(int level) { fortLevel = level; }
		void setNavalBaseLevel(int level) { navalBaseLevel = level; }
		bool hasNavalBase() const { return (navalBaseLevel > 0); }
		int getNavalBaseLevel() const { return navalBaseLevel; }
		std::optional<std::shared_ptr<Factory>> addFactory(std::shared_ptr<Factory> factory);
		bool hasLandConnection() const { return landConnection; }
		void setLandConnection(bool _connection) { landConnection = _connection; }
		int getMfgCount() const { return mfgCount; }
		bool isColony() const { return colonial != 0; }
		void addPopDemographic(const Demographic& d);
		const std::set<int>& getEU4IDs() const { return eu4IDs; }
		void setSameContinent() { sameContinent = true; }
		int getTotalPopulation() const;
		bool wasColony() const { return wasColonised; }
		std::vector<std::string> getCulturesOverThreshold(double percentOfPopulation) const;
		std::string getFilename() const { return filename; }
		std::optional<std::pair<int, std::vector<std::shared_ptr<Pop>>>> getPopsForOutput();
		std::string getRegimentName(REGIMENTTYPE rc);

		void determineColonial();
		void convertFromOldProvince(
			const std::vector<std::shared_ptr<EU4::Province>>& provinceSources,
			const std::map<std::string, std::shared_ptr<EU4::Country>>& theEU4Countries,
			const EU4::Regions& eu4Regions
		);
		void doCreatePops(
			double popWeightRatio,
			Country* _owner,
			int popConversionAlgorithm,
			const mappers::ProvinceMapper& provinceMapper
		);
		std::shared_ptr<Pop> getSoldierPopForArmy(bool force = false);
		std::vector<std::shared_ptr<Pop>> getPops(const std::string& type) const;
		std::pair<int, int>	getAvailableSoldierCapacity() const;
		
		friend std::ostream& operator<<(std::ostream& output, const Province& province);

	private:
		int provinceID = 0;
		std::string name;
		std::string filename;
		mappers::ProvinceDetails details;
		std::vector<std::shared_ptr<Pop>> vanillaPops;
		std::vector<std::shared_ptr<Pop>> minorityPops;
		std::vector<std::shared_ptr<Pop>> pops;
		int vanillaPopulation = 0;
		double slaveProportion = 0.0;
		bool coastal = false;
		bool resettable = false;
		bool territorialCore = false;
		bool inHRE = false;
		int colonial = 0;
		bool wasColonised = false;
		int fortLevel = 0;
		int navalBaseLevel = 0;
		int railLevel = 0;
		std::map<std::string, std::shared_ptr<Factory>> factories;
		bool landConnection = false;
		int mfgCount = 0;
		std::vector<Demographic> demographics;
		std::set<int> eu4IDs; // Source province IDs, fuzzy at best since we mangled them together to craft this province.
		bool sameContinent = false;
		double devpushMod = 0.0;
		double weightMod = 0.0;
		double totalWeight = 0.0;
		std::set<std::string> importedBuildings;
		std::set<std::string> importedIdeas;
		std::map<REGIMENTTYPE, int> unitNameCount;

		struct pop_points
		{
			double craftsmen = 0;
			double slaves = 0;
			double soldiers = 0;
			double artisans = 0;
			double clergymen = 0;
			double clerks = 0;
			double bureaucrats = 0;
			double officers = 0;
			double capitalists = 0;
			double aristocrats = 0;
		};

		void determineDemographics(
			const EU4::Regions& eu4Regions,
			std::vector<EU4::PopRatio>& popRatios,
			int eu4ProvID,
			std::string oldOwnerTag,
			int destNum,
			double provPopRatio);
		pop_points getPopPoints_1(
			const Demographic& demographic,
			double newPopulation,
			const Country* _owner) const; // EU4 1.0-1.11
		pop_points getPopPoints_2(
			const Demographic& demographic,
			double newPopulation,
			const Country* _owner) const; // EU4 1.12 and newer
		void createPops(
			const Demographic& demographic,
			double popWeightRatio,
			const Country* _owner,
			int popConversionAlgorithm,
			const mappers::ProvinceMapper& provinceMapper);
		void combinePops();
		static bool popSortBySizePredicate(std::shared_ptr<Pop> pop1, std::shared_ptr<Pop> pop2);
		static int getRequiredPopForRegimentCount(int count);
		bool growSoldierPop(std::shared_ptr<Pop> pop);
	};	

	std::ostream& operator<<(std::ostream& output, const std::optional<std::pair<int, std::vector<std::shared_ptr<V2::Pop>>>>& pops);
}

#endif // PROVINCE_H
