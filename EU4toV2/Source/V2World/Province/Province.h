#ifndef PROVINCE_H
#define PROVINCE_H

#include <string>
#include "../../Mappers/ProvinceDetails/ProvinceDetails.h"
#include "../../Mappers/Geography/ClimateMapper.h"
#include "../../Mappers/Geography/TerrainDataMapper.h"
#include "../Pop/Pop.h"
#include "../../EU4World/Religions/Religions.h"
#include "../../EU4World/Provinces/EU4Province.h"
#include "../../EU4World/Country/EU4Country.h"
#include "../Factory/Factory.h"

namespace V2
{
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
		Province(const std::string& _filename, const mappers::ClimateMapper& climateMapper, const mappers::TerrainDataMapper& terrainDataMapper);
		int getID() const { return provinceID; }
		std::string getRgoType() const { return details.rgoType; }
		std::string getClimate() const { return details.climate; }
		std::string getTerrain() const { return details.terrain; }
		void setRgoType(const std::string& _type) { details.rgoType = _type; }
		void setName(std::string _name) { name = _name; }
		void setOwner(std::string _owner) { details.owner = _owner; }
		void setController(std::string _controller) { details.controller = _controller; }
		std::string getName() const { return name; }
		std::string getOwner() const { return details.owner; }
		std::string getController() const { return details.controller; }
		void setCoastal() { coastal = true; }
		bool isCoastal() const { return coastal; }
		void setResettable() { resettable = true; }
		bool getResettable() const { return resettable; }
		void clearCores() { details.cores.clear(); }
		void addCore(std::string);
		bool isTerritorialCore() const { return territorialCore; }
		void addVanillaPop(std::shared_ptr<Pop> vanillaPop);
		void addMinorityPop(std::shared_ptr<Pop> minorityPop);
		void setSlaveProportion(const double _pro) { slaveProportion = _pro; }
		void setRailLevel(int level) { railLevel = level; }
		void setFortLevel(int level) { fortLevel = level; }
		void setNavalBaseLevel(int level) { navalBaseLevel = level; }
		bool hasNavalBase() const { return (navalBaseLevel > 0); }
		int getNavalBaseLevel() const { return navalBaseLevel; }
		void addFactory(const Factory& factory);
		bool hasLandConnection() const { return landConnection; }
		void setLandConnection(bool _connection) { landConnection = _connection; }
		int getMfgCount() const { return mfgCount; }
		bool isColony() const { return colonial != 0; }
		void addPopDemographic(const Demographic& d);
		int getEU4ID() const { return eu4ID; }
		void setSameContinent() { sameContinent = true; }
		int getTotalPopulation() const;
		bool wasColony() const { return wasColonised; }
		std::vector<std::string> getCulturesOverThreshold(double percentOfPopulation) const;
		std::string getFilename() const { return filename; }
		std::optional<std::pair<int, std::vector<std::shared_ptr<Pop>>>> getPopsForOutput();

		void determineColonial();
		void convertFromOldProvince(
			const EU4::Religions& allReligions,
			const EU4::Province* oldProvince,
			const std::map<std::string, std::shared_ptr<EU4::Country>>& theEU4Countries
		);

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
		std::map<std::string, Factory> factories;
		bool landConnection = false;
		int mfgCount = 0;
		std::vector<Demographic> demographics;
		int eu4ID = 0; // Source province ID, fuzzy at best.
		bool sameContinent = false;
	};
	
}

/*
#include "../EU4World/World.h"
#include "../EU4World/Country/EU4Country.h"
#include "../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "newParser.h"
#include "Factory/Factory.h"
#include "Pop/Pop.h"
#include "Army/Regiment.h"

class V2Country;



class V2Province : commonItems::parser
{
	public:
		V2Province(const std::string& _filename);
		void output() const;
		void outputPops(std::ofstream& output) const;
		
		void doCreatePops(
			double popWeightRatio,
			V2Country* _owner,
			int popConversionAlgorithm,
			const std::map<std::string, std::shared_ptr<EU4::Country>>& theEU4Countries,
			const mappers::ProvinceMapper& provinceMapper
		);

		std::vector<V2::Pop*> getPops(const std::string& type) const;
		V2::Pop* getSoldierPopForArmy(bool force = false);
		std::pair<int, int>	getAvailableSoldierCapacity() const;
		std::string getRegimentName(V2::REGIMENTTYPE rc);
		bool hasCulture(const std::string& culture, float percentOfPopulation) const;

		
		void setClimate(std::string _climate) { climate = _climate; }
		
		void setTerrain(std::string _terrain) { terrain = _terrain; }

		const EU4::Province*	getSrcProvince() const { return srcProvince; }
		int getOldPopulation() const	{ return oldPopulation; }
		
		
		std::vector<V2::Pop*> getPops() const { return pops; }

	private:
		void outputUnits(FILE*) const;

		struct pop_points;
		pop_points getPopPoints_1(
			const V2Demographic& demographic,
			double newPopulation,
			const V2Country* _owner,
			const std::map<std::string, std::shared_ptr<EU4::Country>>& theEU4Countries); // EU4 1.0-1.11
		pop_points getPopPoints_2(
			const V2Demographic& demographic,
			double newPopulation,
			const V2Country* _owner,
			const std::map<std::string, std::shared_ptr<EU4::Country>>& theEU4Countries
		); // EU4 1.12 and newer
		void createPops(
			const V2Demographic& demographic,
			double popWeightRatio,
			const V2Country* _owner,
			int popConversionAlgorithm,
			const std::map<std::string, std::shared_ptr<EU4::Country>>& theEU4Countries,
			const mappers::ProvinceMapper& provinceMapper
		);
		void combinePops();
		bool growSoldierPop(V2::Pop* pop);

		const EU4::Province* srcProvince = NULL;

		std::string filename;
		bool coastal = false;
		int num = 0;
		std::string name;
		std::string owner;
		std::string controller;
		std::vector<std::string> cores;
		int colonyLevel = 0;
		int oldPopulation = 0;
		std::string rgoType;
		std::string terrain;
		std::string climate;
		int lifeRating = 0;
		bool slaveState = false;
		std::map<V2::REGIMENTTYPE, int> unitNameCount;

		bool resettable = false;
		double spentProvinceModifier = 0; //Store old popshaping modifier for NEU4-to-1V2 conversions;
};


*/
#endif // PROVINCE_H
