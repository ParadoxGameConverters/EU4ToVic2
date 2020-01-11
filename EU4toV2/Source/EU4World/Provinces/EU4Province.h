#ifndef EU4PROVINCE_H_
#define EU4PROVINCE_H_

#include "Date.h"
#include "BuildingWeightEffects.h"
#include "PopRatio.h"
#include "ProvinceBuildings.h"
#include "ProvinceHistory.h"
#include "ProvinceStats.h"
#include "../../Mappers/Buildings/Buildings.h"
#include "../Modifiers/Modifiers.h"
#include "newParser.h"
#include <map>

namespace EU4
{
	class Province: commonItems::parser
	{
	public:
		Province(
			const std::string& numString,
			std::istream& theStream,
			const mappers::Buildings& buildingTypes,
			const Modifiers& modifierTypes
		);

		bool hasBuilding(const std::string& building) const;
		bool hasGreatProject(const std::string& greatProject) const;
		double getCulturePercent(const std::string& culture) const;

		std::string getArea() const { return areaName; }
		std::string getName() const { return name; }
		std::string getOwnerString() const { return ownerString; }
		std::string getControllerString() const { return controllerString; }
		const std::set<std::string>& getCores() const { return cores; }
		const std::vector<EU4::PopRatio>& getPopRatios() const { return provinceHistory.getPopRatios(); }
		std::optional<date> getFirstOwnedDate() const { return provinceHistory.getFirstOwnedDate(); }
		int getNum() const { return num; }
		bool inHre() const { return inHRE; }
		bool isTerritorialCore() const { return territorialCore; }
		bool isColony() const { return colony; }
		bool isCity() const { return city; }
		bool isState() const { return stated; }
		bool wasColonised() const { return hadOriginalColoniser || provinceHistory.wasColonized(); }
		bool hasModifier(const std::string& modifierName) const { return modifiers.count(modifierName) > 0; }

		void addCore(const std::string& tag) { cores.insert(tag); }
		void removeCore(const std::string& tag) { cores.erase(tag); }
		void setOwnerString(const std::string& _owner) { ownerString = _owner; }
		void setControllerString(const std::string& _controller) { controllerString = _controller; }

		// getters for weight attributes
		double getTotalWeight() const { return totalWeight; }
		double getBaseTax() const { return baseTax; }
		double getTaxIncome() const { return taxIncome; }
		double getProductionIncome() const { return productionIncome; }
		double getManpowerWeight() const { return manpowerWeight; }
		double getTotalBuildingWeight() const { return buildingWeight; }
		double getTotalDevModifier() const { return devModifier; }
		double getDevDelta() const { return devDelta; }
		double getModifierWeight() const { return modifierWeight; }
		const ProvinceStats& getProvinceStats() const { return provinceStats; }
		std::string getTradeGoods() const { return tradeGoods; }
		double getProsperity() const { return prosperity; }

		void setArea(const std::string& a) { areaName = a; }

	private:
		void determineProvinceWeight(const mappers::Buildings& buildingTypes, const Modifiers& modifierTypes);
		BuildingWeightEffects getProvBuildingWeight(const mappers::Buildings& buildingTypes, const Modifiers& modifierTypes) const;
		double getTradeGoodPrice() const;

		int num = 0;
		std::string	name;
		std::string ownerString;
		std::string controllerString;
		std::set<std::string> cores;

		bool inHRE = false;
		bool colony = false;
		bool hadOriginalColoniser = false;
		bool territorialCore = false;
		bool city = false;

		EU4::ProvinceHistory provinceHistory;
		EU4::ProvinceBuildings buildings;
		std::set<std::string> greatProjects;
		std::set<std::string> modifiers;

		// province attributes for weights
		double baseTax = 0.0;
		double baseProduction = 0.0;
		double manpower = 0.0;
		double totalWeight = 0.0;
		std::string tradeGoods;
		std::string areaName;
		double taxIncome = 0;
		double productionIncome = 0;
		double manpowerWeight = 0;
		double buildingWeight = 0;
		double devModifier = 0;
		double devDelta = 0;
		double modifierWeight = 0;
		int centerOfTradeLevel = 0;
		ProvinceStats provinceStats;
		double prosperity = 0;
		bool stated = false;
	};

}

#endif // EU4PROVINCE_H_
