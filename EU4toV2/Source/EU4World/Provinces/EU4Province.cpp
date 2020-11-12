#include "EU4Province.h"
#include "Configuration.h"
#include "Country/EU4Country.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "ProvinceModifier.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <optional>

const double BUILDING_COST_TO_WEIGHT_RATIO = 0.02;

EU4::Province::Province(const std::string& numString, std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();

	num = 0 - stoi(numString);

	if (!provinceHistory.hasInitializedHistory() && !culture.empty() && !religion.empty())
	{
		// recover from broken save data.
		provinceHistory.setStartingCulture(culture);
		provinceHistory.setStartingReligion(religion);
	} // Else it's probably a blank province anyway.

	// for old versions of EU4 (< 1.12), copy tax to production if necessary
	if (baseProduction == 0.0 && baseTax > 0)
		baseProduction = baseTax;
}

void EU4::Province::registerKeys()
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		name = commonItems::singleString(theStream).getString();
	});
	registerKeyword("culture", [this](const std::string& unused, std::istream& theStream) {
		culture = commonItems::singleString(theStream).getString();
	});
	registerKeyword("religion", [this](const std::string& unused, std::istream& theStream) {
		religion = commonItems::singleString(theStream).getString();
	});
	registerKeyword("base_tax", [this](const std::string& unused, std::istream& theStream) {
		baseTax = commonItems::singleDouble(theStream).getDouble();
	});
	registerKeyword("base_production", [this](const std::string& unused, std::istream& theStream) {
		baseProduction = commonItems::singleDouble(theStream).getDouble();
	});
	registerKeyword("base_manpower", [this](const std::string& unused, std::istream& theStream) {
		manpower = commonItems::singleDouble(theStream).getDouble();
	});
	registerKeyword("manpower", [this](const std::string& unused, std::istream& theStream) {
		manpower = commonItems::singleDouble(theStream).getDouble();
	});
	registerKeyword("owner", [this](const std::string& unused, std::istream& theStream) {
		ownerString = commonItems::singleString(theStream).getString();
	});
	registerKeyword("controller", [this](const std::string& unused, std::istream& theStream) {
		controllerString = commonItems::singleString(theStream).getString();
	});
	registerKeyword("cores", [this](const std::string& unused, std::istream& theStream) {
		const auto& coreList = commonItems::stringList(theStream).getStrings();
		cores.insert(coreList.begin(), coreList.end());
	});
	registerKeyword("core", [this](const std::string& unused, std::istream& theStream) {
		cores.insert(commonItems::singleString(theStream).getString());
	});
	registerKeyword("territorial_core", [this](const std::string& unused, std::istream& theStream) {
		commonItems::ignoreItem(unused, theStream);
		territorialCore = true;
	});
	registerKeyword("hre", [this](const std::string& unused, std::istream& theStream) {
		inHRE = commonItems::singleString(theStream).getString() == "yes";
	});
	registerKeyword("is_city", [this](const std::string& unused, std::istream& theStream) {
		city = commonItems::singleString(theStream).getString() == "yes";
	});
	registerKeyword("colonysize", [this](const std::string& unused, std::istream& theStream) {
		commonItems::ignoreItem(unused, theStream);
		colony = true;
	});
	registerKeyword("original_coloniser", [this](const std::string& unused, std::istream& theStream) {
		commonItems::ignoreItem(unused, theStream);
		hadOriginalColonizer = true;
	});
	registerKeyword("history", [this](const std::string& unused, std::istream& theStream) {
		const ProvinceHistory theHistory(theStream);
		provinceHistory = theHistory;
	});
	registerKeyword("buildings", [this](const std::string& unused, std::istream& theStream) {
		const ProvinceBuildings theBuildings(theStream);
		buildings = theBuildings;
	});
	registerKeyword("great_projects", [this](const std::string& unused, std::istream& theStream) {
		const auto& projectList = commonItems::stringList(theStream).getStrings();
		greatProjects.insert(projectList.begin(), projectList.end());
	});
	registerKeyword("modifier", [this](const std::string& unused, std::istream& theStream) {
		const ProvinceModifier modifier(theStream);
		modifiers.insert(modifier.getModifier());
	});
	registerKeyword("trade_goods", [this](const std::string& unused, std::istream& theStream) {
		tradeGoods = commonItems::singleString(theStream).getString();
	});
	registerKeyword("center_of_trade", [this](const std::string& unused, std::istream& theStream) {
		centerOfTradeLevel = commonItems::singleInt(theStream).getInt();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void EU4::Province::buildPopRatio(const mappers::SuperGroupMapper& superGroupMapper, const Regions& regions)
{
	double assimilationFactor;
	const auto& superRegionName = regions.getParentSuperRegionName(num);
	if (superRegionName)
		assimilationFactor = superGroupMapper.getAssimilationFactor(*superRegionName);
	else
		assimilationFactor = 0.0025;
	provinceHistory.buildPopRatios(assimilationFactor);
}

bool EU4::Province::hasBuilding(const std::string& building) const
{
	return buildings.hasBuilding(building);
}

bool EU4::Province::hasGreatProject(const std::string& greatProject) const
{
	return greatProjects.count(greatProject);
}

double EU4::Province::getCulturePercent(const std::string& theCulture) const
{
	auto culturePercent = 0.0;
	for (const auto& pop: provinceHistory.getPopRatios())
		if (pop.getCulture() == theCulture)
			culturePercent += pop.getLowerRatio();

	return culturePercent;
}

void EU4::Province::determineProvinceWeight(const mappers::Buildings& buildingTypes, const Modifiers& modifierTypes)
{
	auto manpower_weight = manpower;
	const auto taxEfficiency = 1.0;

	const auto buildingWeightEffects = getProvBuildingWeight(buildingTypes, modifierTypes);
	buildingWeight = buildingWeightEffects.buildingWeight;
	const auto manpowerModifier = buildingWeightEffects.manpowerModifier;
	const auto manufactoriesValue = buildingWeightEffects.manufactoriesValue;
	const auto productionEfficiency = buildingWeightEffects.productionEfficiency;
	const auto taxModifier = buildingWeightEffects.taxModifier;
	const auto tradeGoodsSizeModifier = buildingWeightEffects.tradeGoodsSizeModifier;
	const auto tradeValue = buildingWeightEffects.tradeValue;
	const auto tradeEfficiency = buildingWeightEffects.tradeEfficiency;
	auto goodsProduced = (baseProduction * 0.2) + manufactoriesValue + tradeGoodsSizeModifier + 0.03;
	goodsProduced = std::max(0.0, goodsProduced);

	// manpower
	manpower_weight *= 25;
	manpower_weight += manpowerModifier;
	manpower_weight *= ((1 + manpowerModifier) / 25); // should work now as intended

	auto total_tx = (baseTax + taxModifier) * (taxEfficiency + 0.15);
	const auto production_eff_tech = 0.5; // used to be 1.0
	const auto total_trade_value = (tradeGoodsPrice * goodsProduced + tradeValue) * (1 + tradeEfficiency);
	auto production_income = total_trade_value * (1 + production_eff_tech + productionEfficiency);

	total_tx *= 1.5;
	manpower_weight *= 1;
	production_income *= 1.5;

	taxIncome = total_tx;
	productionIncome = production_income;
	manpowerWeight = manpower_weight;

	// dev modifier
	devModifier = baseTax + baseProduction + manpower;
	devDelta = devModifier - provinceHistory.getOriginalDevelopment();
	modifierWeight = buildingWeight + manpower_weight + production_income + total_tx;

	totalWeight = devModifier + modifierWeight;

	if (modifierWeight > 0)
	{
		// provinces with modifier weights under 10 (underdeveloped with no buildings) get a penalty for popShaping.
		modifierWeight = (std::log10(modifierWeight) - 1) * 10;
	}

	if (ownerString.empty())
	{
		totalWeight = 0;
		modifierWeight = 0;
	}

	provinceStats.goodsProduced = goodsProduced;
	provinceStats.price = tradeGoodsPrice;
	provinceStats.tradeEfficiency = 1 + tradeEfficiency;
	provinceStats.productionEfficiency = 1 + productionEfficiency;
	provinceStats.tradeValue = tradeValue;
	provinceStats.baseTax = baseTax;
	provinceStats.buildingsIncome = taxModifier;
	provinceStats.taxEfficiency = taxEfficiency;
	provinceStats.totalTaxIncome = total_tx;
	provinceStats.totalTradeValue = total_trade_value;
}

EU4::BuildingWeightEffects EU4::Province::getProvBuildingWeight(const mappers::Buildings& buildingTypes, const Modifiers& modifierTypes) const
{
	BuildingWeightEffects effects;

	for (const auto& buildingName: buildings.getBuildings())
	{
		auto theBuilding = buildingTypes.getBuilding(buildingName);
		if (theBuilding)
		{
			effects.buildingWeight += theBuilding->getCost() * BUILDING_COST_TO_WEIGHT_RATIO;
			if (theBuilding->isManufactory())
				effects.manufactoriesValue += 1.0;

			for (const auto& effect: theBuilding->getModifier().getAllEffects())
				if (effect.first == "local_manpower_modifier")
					effects.manpowerModifier += effect.second;
				else if (effect.first == "local_tax_modifier")
					effects.taxModifier += effect.second;
				else if (effect.first == "local_production_efficiency")
					effects.productionEfficiency += effect.second;
				else if (effect.first == "province_trade_power_modifier")
					effects.tradePower += effect.second;
				else if (effect.first == "trade_efficiency")
					effects.tradeEfficiency += effect.second;
				else if (effect.first == "trade_goods_size" || effect.first == "trade_goods_size_modifier")
					effects.tradeGoodsSizeModifier += effect.second;
				else if (effect.first == "trade_value_modifier")
					effects.tradeValue += effect.second;
				else if (effect.first == "trade_steering")
					effects.tradeSteering += effect.second;
		}
		else
		{
			LOG(LogLevel::Warning) << "Could not look up information for building type " << buildingName;
		}
	}

	for (const auto& modifierName: modifiers)
	{
		auto theModifier = modifierTypes.getModifier(modifierName);
		if (theModifier)
		{
			for (const auto& effect: theModifier->getAllEffects())
				if (effect.first == "local_manpower_modifier")
					effects.manpowerModifier += effect.second;
				else if (effect.first == "local_tax_modifier")
					effects.taxModifier += effect.second;
				else if (effect.first == "local_production_efficiency")
					effects.productionEfficiency += effect.second;
				else if (effect.first == "province_trade_power_modifier")
					effects.tradePower += effect.second;
				else if (effect.first == "trade_efficiency")
					effects.tradeEfficiency += effect.second;
				else if (effect.first == "trade_goods_size" || effect.first == "trade_goods_size_modifier")
					effects.tradeGoodsSizeModifier += effect.second;
				else if (effect.first == "trade_value_modifier")
					effects.tradeValue += effect.second;
				else if (effect.first == "trade_steering")
					effects.tradeSteering += effect.second;
		}
		else
		{
			LOG(LogLevel::Warning) << "Could not look up information for modifier type " << modifierName;
		}
	}

	if (centerOfTradeLevel == 1)
		effects.tradePower += 5;
	else if (centerOfTradeLevel == 2)
		effects.tradePower += 10;
	else if (centerOfTradeLevel == 3)
		effects.tradePower += 25;

	return effects;
}
