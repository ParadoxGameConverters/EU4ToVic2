#ifndef EU4_PROVINCE_H
#define EU4_PROVINCE_H
#include "BuildingWeightEffects.h"
#include "Buildings/Buildings.h"
#include "Date.h"
#include "Modifiers/Modifiers.h"
#include "Parser.h"
#include "PopRatio.h"
#include "ProvinceBuildings.h"
#include "ProvinceHistory.h"
#include "ProvinceStats.h"
#include "Regions/Regions.h"
#include "SuperGroupMapper/SuperGroupMapper.h"

namespace EU4
{
class Province: commonItems::parser
{
  public:
	Province(const std::string& numString, std::istream& theStream);

	[[nodiscard]] auto getNum() const { return num; }
	[[nodiscard]] auto inHre() const { return inHRE; }
	[[nodiscard]] auto isTerritorialCore() const { return territorialCore; }
	[[nodiscard]] auto isColony() const { return colony; }
	[[nodiscard]] auto isCity() const { return city; }
	[[nodiscard]] auto isState() const { return stated; }
	[[nodiscard]] auto wasColonized() const { return hadOriginalColonizer || provinceHistory.wasColonized(); }
	[[nodiscard]] auto hasModifier(const std::string& modifierName) const { return modifiers.count(modifierName); }
	[[nodiscard]] auto getTotalWeight() const { return totalWeight; }
	[[nodiscard]] auto getBaseTax() const { return baseTax; }
	[[nodiscard]] auto getTaxIncome() const { return taxIncome; }
	[[nodiscard]] auto getProductionIncome() const { return productionIncome; }
	[[nodiscard]] auto getManpowerWeight() const { return manpowerWeight; }
	[[nodiscard]] auto getTotalBuildingWeight() const { return buildingWeight; }
	[[nodiscard]] auto getTotalDevModifier() const { return devModifier; }
	[[nodiscard]] auto getDevDelta() const { return devDelta; }
	[[nodiscard]] auto getModifierWeight() const { return modifierWeight; }
	[[nodiscard]] auto getProsperity() const { return prosperity; }
	[[nodiscard]] auto getFirstOwnedDate() const { return provinceHistory.getFirstOwnedDate(); }

	[[nodiscard]] const auto& getArea() const { return areaName; }
	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getOwnerString() const { return ownerString; }
	[[nodiscard]] const auto& getControllerString() const { return controllerString; }
	[[nodiscard]] const auto& getOriginalCulture() const { return provinceHistory.getStartingCulture(); }
	[[nodiscard]] const auto& getTradeGoods() const { return tradeGoods; }
	[[nodiscard]] const auto& getProvinceStats() const { return provinceStats; }
	[[nodiscard]] const auto& getCores() const { return cores; }
	[[nodiscard]] const auto& getPopRatios() const { return provinceHistory.getPopRatios(); }
	[[nodiscard]] const auto& exportBuildings() const { return buildings.getBuildings(); }

	[[nodiscard]] bool hasBuilding(const std::string& building) const;
	[[nodiscard]] bool hasGreatProject(const std::string& greatProject) const;
	[[nodiscard]] double getCulturePercent(const std::string& theCulture) const;

	void addCore(const std::string& tag) { cores.insert(tag); }
	void removeCore(const std::string& tag) { cores.erase(tag); }
	void setOwnerString(const std::string& _owner) { ownerString = _owner; }
	void setControllerString(const std::string& _controller) { controllerString = _controller; }
	void setTradeGoodPrice(double price) { tradeGoodsPrice = price; }
	void setArea(const std::string& theArea) { areaName = theArea; }
	void determineProvinceWeight(const mappers::Buildings& buildingTypes, const Modifiers& modifierTypes);
	void updatePopRatioCulture(const std::string& oldCultureName, const std::string& neoCultureName, const std::string& superRegion)
	{
		provinceHistory.updatePopRatioCulture(oldCultureName, neoCultureName, superRegion);
	}
	void buildPopRatio(const mappers::SuperGroupMapper& superGroupMapper, const Regions& regions);

  private:
	void registerKeys();
	[[nodiscard]] BuildingWeightEffects getProvBuildingWeight(const mappers::Buildings& buildingTypes, const Modifiers& modifierTypes) const;

	int num = 0;
	std::string name;
	std::string ownerString;
	std::string controllerString;
	std::string culture;
	std::string religion;

	std::set<std::string> cores;

	bool inHRE = false;
	bool colony = false;
	bool hadOriginalColonizer = false;
	bool territorialCore = false;
	bool city = false;
	bool stated = false;
	int centerOfTradeLevel = 0;

	ProvinceHistory provinceHistory;
	ProvinceBuildings buildings;

	std::set<std::string> greatProjects;
	std::set<std::string> modifiers;
	std::string tradeGoods;
	std::string areaName;

	// province attributes for weights
	double baseTax = 0;
	double baseProduction = 0;
	double manpower = 0;
	double totalWeight = 0;
	double tradeGoodsPrice = 0;
	double taxIncome = 0;
	double productionIncome = 0;
	double manpowerWeight = 0;
	double buildingWeight = 0;
	double devModifier = 0;
	double devDelta = 0;
	double modifierWeight = 0;
	double prosperity = 0;
	ProvinceStats provinceStats;
};
} // namespace EU4

#endif // EU4_PROVINCE_H
