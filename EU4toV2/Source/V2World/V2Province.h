#ifndef V2PROVINCE_H_
#define V2PROVINCE_H_

#include "../Configuration.h"
#include "../EU4World/World.h"
#include "../EU4World/Country/EU4Country.h"
#include "../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "newParser.h"

class V2Pop;
class V2Factory;
class V2Country;

struct V2Demographic
{
	std::string culture;
	std::string slaveCulture;
	std::string religion;
	double upperRatio;
	double middleRatio;
	double lowerRatio;
	const EU4::Province* oldProvince;
	std::string oldCountry;
};


class V2Province : commonItems::parser
{
	public:
		V2Province(const std::string& _filename);
		void output() const;
		void outputPops(FILE*) const;
		void convertFromOldProvince(
			const EU4::Religions& allReligions,
			const EU4::Province* oldProvince,
			const std::map<std::string, std::shared_ptr<EU4::Country>>& theEU4Countries
		);
		void determineColonial();
		void addCore(std::string);
		void addOldPop(const V2Pop*);
		void addMinorityPop(V2Pop*);
		void doCreatePops(
			double popWeightRatio,
			V2Country* _owner,
			int popConversionAlgorithm,
			const std::map<std::string, std::shared_ptr<EU4::Country>>& theEU4Countries,
			const mappers::ProvinceMapper& provinceMapper
		);
		void addFactory(const V2Factory& factory);
		void addPopDemographic(V2Demographic d);

		int getTotalPopulation() const;
		std::vector<V2Pop*>	getPops(std::string type) const;
		V2Pop* getSoldierPopForArmy(bool force = false);
		std::pair<int, int>	getAvailableSoldierCapacity() const;
		std::string getRegimentName(EU4::REGIMENTCATEGORY rc);
		bool hasCulture(std::string culture, float percentOfPopulation) const;
		std::vector<std::string> getCulturesOverThreshold(float percentOfPopulation) const;

		void clearCores() { cores.clear(); }
		void setCoastal(bool _coastal) { coastal = _coastal; }
		void setClimate(std::string _climate) { climate = _climate; }
		void setName(std::string _name) { name = _name; }
		void setOwner(std::string _owner) { owner = _owner; }
		void setController(std::string _controller) { controller = _controller; }
		void setLandConnection(bool _connection) { landConnection = _connection; }
		void setRgoType(std::string _type) { rgoType = _type; }
		void setSameContinent(bool _same) { sameContinent = _same; }
		void setFortLevel(int level) { fortLevel = level; }
		void setNavalBaseLevel(int level) { navalBaseLevel = level; }
		void setRailLevel(int level) { railLevel = level; }
		void setResettable(const bool _resettable) { resettable = _resettable; }
		void setSlaveProportion(const double _pro) { slaveProportion = _pro; }
		void setTerrain(std::string _terrain) { terrain = _terrain; }

		const EU4::Province*	getSrcProvince() const { return srcProvince; }
		int getOldPopulation() const	{ return oldPopulation; }
		bool wasColony() const { return wasColonised; }
		bool isColonial() const { return colonial != 0; }
		bool isTerritorialCore() const { return territorialCore; }   
		std::string getRgoType() const { return rgoType; }
		std::string getOwner() const { return owner; }
		std::string getController() const { return controller; }
		int getNum() const { return num; }
		std::string getName() const { return name; }
		std::string getClimate() const { return climate; }
		std::string getTerrain() const { return terrain; }
		bool isCoastal() const { return coastal; }
		bool hasNavalBase() const { return (navalBaseLevel > 0); }
		int getNavalBaseLevel() const { return navalBaseLevel; }
		bool hasLandConnection() const { return landConnection; }
		std::vector<V2Pop*> getPops() const { return pops; }

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
		bool growSoldierPop(V2Pop* pop);

		const EU4::Province* srcProvince = NULL;

		std::string filename;
		bool coastal = false;
		int num = 0;
		std::string name;
		std::string owner;
		std::string controller;
		std::vector<std::string> cores;
		bool inHRE = false;
		int colonyLevel = 0;
		int colonial = 0;
		bool wasColonised = false;
		bool landConnection = false;
		bool sameContinent = false;
		bool territorialCore = false;
		int oldPopulation = 0;
		std::vector<V2Demographic> demographics;
		std::vector<const V2Pop*> oldPops;
		std::vector<V2Pop*> minorityPops;
		std::vector<V2Pop*> pops;
		double slaveProportion = 0.0;
		std::string rgoType;
		std::string terrain;
		std::string climate;
		int lifeRating = 0;
		bool slaveState = false;
		int unitNameCount[static_cast<int>(EU4::REGIMENTCATEGORY::num_reg_categories)];
		int fortLevel = 0;
		int navalBaseLevel = 0;
		int railLevel = 0;
		std::map<std::string, V2Factory> factories;

		bool resettable = false;
		double spentProvinceModifier = 0; //Store old popshaping modifier for NEU4-to-1V2 conversions;
};



#endif // V2PROVINCE_H_
