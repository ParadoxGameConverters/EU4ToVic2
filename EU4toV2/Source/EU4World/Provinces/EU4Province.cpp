#include "EU4Province.h"
#include "ProvinceModifier.h"
#include "../Country/EU4Country.h"
#include "../Religions/Religions.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "../../Configuration.h"
#include <algorithm>
#include <fstream>
#include <optional>
#include <sstream>
#include <cmath>

const double BUILDING_COST_TO_WEIGHT_RATIO = 0.02;

EU4::Province::Province(
	const std::string& numString,
	std::istream& theStream,
	const Buildings& buildingTypes,
	const Modifiers& modifierTypes
) {
	registerKeyword(std::regex("name"), [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::singleString nameString(theStream);
			name = nameString.getString();
		});
	registerKeyword(std::regex("base_tax"), [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::singleDouble baseTaxDouble(theStream);
			baseTax = baseTaxDouble.getDouble();
		});
	registerKeyword(std::regex("base_production"), [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::singleDouble baseProductionDouble(theStream);
			baseProduction = baseProductionDouble.getDouble();
		});
	registerKeyword(std::regex("base_manpower"), [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::singleDouble manpowerDouble(theStream);
			manpower = manpowerDouble.getDouble();
		});
	registerKeyword(std::regex("manpower"), [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::singleDouble manpowerDouble(theStream);
			manpower = manpowerDouble.getDouble();
		});
	registerKeyword(std::regex("owner"), [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::singleString ownerStringString(theStream);
			ownerString = ownerStringString.getString();
		});
	registerKeyword(std::regex("controller"), [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::singleString controllerStringString(theStream);
			controllerString = controllerStringString.getString();
		});
	registerKeyword(std::regex("cores"), [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::stringList coresStrings(theStream);
			for (auto coreString : coresStrings.getStrings()) cores.insert(coreString);
		});
	registerKeyword(std::regex("core"), [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::singleString coresString(theStream);
			cores.insert(coresString.getString());
		});
    registerKeyword(std::regex("territorial_core"),[this](const std::string& unused, std::istream& theStream) 
		 {
			commonItems::ignoreItem(unused, theStream);
			territorialCore = true;
		});
	registerKeyword(std::regex("hre"), [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::ignoreItem(unused, theStream);
			inHRE = true;
		});
	registerKeyword(std::regex("is_city"), [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::ignoreItem(unused, theStream);
			city = true;
		});
	registerKeyword(std::regex("colonysize"), [this](const std::string & unused, std::istream & theStream) 
		{
			commonItems::ignoreItem(unused, theStream);
			colony = true;
	});
	registerKeyword(std::regex("original_coloniser"), [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::ignoreItem(unused, theStream);
			hadOriginalColoniser = true;
		});
	registerKeyword(std::regex("history"), [this](const std::string& unused, std::istream& theStream) 
		{
			ProvinceHistory theHistory(theStream);
			provinceHistory = theHistory;
		});
	registerKeyword(std::regex("buildings"), [this](const std::string& unused, std::istream& theStream) 
		{
			ProvinceBuildings theBuildings(theStream);
			buildings = theBuildings;
		});
	registerKeyword(std::regex("great_projects"), [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::stringList theProjects(theStream);
			for (const auto& project : theProjects.getStrings()) greatProjects.insert(project);
		});
	registerKeyword(std::regex("modifier"), [this](const std::string& unused, std::istream& theStream) 
		{
			ProvinceModifier modifier(theStream);
			modifiers.insert(modifier.getModifier());
		});
	registerKeyword(std::regex("trade_goods"), [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::singleString tradeGoodsString(theStream);
			tradeGoods = tradeGoodsString.getString();
		});
	registerKeyword(std::regex("center_of_trade"), [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::singleInt cotLevelInt(theStream);
			centerOfTradeLevel = cotLevelInt.getInt();
		});
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseStream(theStream);

	num = 0 - stoi(numString);

	// for old versions of EU4 (< 1.12), copy tax to production if necessary
	if ((baseProduction == 0.0f) && (baseTax > 0.0f))
	{
		baseProduction = baseTax;
	}

	determineProvinceWeight(buildingTypes, modifierTypes);
}

bool EU4::Province::hasBuilding(const std::string& building) const
{
	if (buildings.hasBuilding(building)) return true;
	return false;
}

bool EU4::Province::hasGreatProject(const std::string& greatProject) const
{
	if (greatProjects.count(greatProject) > 0) return true;
	return false;
}

double EU4::Province::getCulturePercent(const std::string& culture) const
{
	double culturePercent = 0.0f;
	for (auto pop: provinceHistory.getPopRatios())
	{
		if (pop.getCulture() == culture) culturePercent += pop.getLowerRatio();
	}

	return culturePercent;
}

void EU4::Province::determineProvinceWeight(const Buildings& buildingTypes, const Modifiers& modifierTypes)
{
	double manpower_weight = manpower;
	double taxEfficiency = 1.0;

	BuildingWeightEffects buildingWeightEffects = getProvBuildingWeight(buildingTypes, modifierTypes);
	buildingWeight = buildingWeightEffects.buildingWeight;
	double manpowerModifier = buildingWeightEffects.manpowerModifier;
	double manufactoriesValue = buildingWeightEffects.manufactoriesValue;
	double productionEfficiency = buildingWeightEffects.productionEfficiency;
	double taxModifier = buildingWeightEffects.taxModifier;
	double tradeGoodsSizeModifier = buildingWeightEffects.tradeGoodsSizeModifier;
	double tradePower = buildingWeightEffects.tradePower;
	double tradeValue = buildingWeightEffects.tradeValue;
	double tradeEfficiency = buildingWeightEffects.tradeEfficiency;
	double tradeSteering = buildingWeightEffects.tradeSteering;
	double goodsProduced = (baseProduction * 0.2) + manufactoriesValue + tradeGoodsSizeModifier + 0.03;
	goodsProduced = std::max(0.0, goodsProduced);

	// manpower
	manpower_weight *= 25;
	manpower_weight += manpowerModifier;
	manpower_weight *= ((1 + manpowerModifier) / 25); // should work now as intended

	double total_tx = (baseTax + taxModifier) * (taxEfficiency + 0.15);
	double production_eff_tech = 0.5; // used to be 1.0

	double total_trade_value = ((getTradeGoodPrice() * goodsProduced) + tradeValue) * (1 + tradeEfficiency);
	double production_income = total_trade_value * (1 + production_eff_tech + productionEfficiency);

	total_tx *= 1.5;
	manpower_weight *= 1;
	production_income *= 1.5;

	taxIncome = total_tx;
	productionIncome = production_income;
	manpowerWeight = manpower_weight;
	
	// dev modifier
	devModifier = ( baseTax + baseProduction + manpower );
	devDelta = devModifier - provinceHistory.getOriginalDevelopment();
	modifierWeight = buildingWeight + manpower_weight + production_income + total_tx;

	totalWeight = devModifier + modifierWeight;

	if (modifierWeight > 0)
	{
		// provinces with modifierweights under 10 (underdeveloped with no buildings) get a penalty for popShaping.
		modifierWeight = (std::log10(modifierWeight) - 1) * 10;
	}

	if (ownerString == "")
	{
		totalWeight = 0;
		modifierWeight = 0;
	}

	provinceStats.setGoodsProduced(goodsProduced);
	provinceStats.setPrice(getTradeGoodPrice());
	provinceStats.setTradeEfficiency(1 + tradeEfficiency);
	provinceStats.setProductionEfficiency(1 + productionEfficiency);
	provinceStats.setTradeValue(tradeValue);
	provinceStats.setTradeValue(production_income);
	provinceStats.setBaseTax(baseTax);
	provinceStats.setBuildingsIncome(taxModifier);
	provinceStats.setTaxEfficiency(taxEfficiency);
	provinceStats.setTotalTaxIncome(total_tx);
	provinceStats.setTotalTradeValue(total_trade_value);
}


double EU4::Province::getTradeGoodPrice() const
{
	// Trade goods
	/*
	chinaware
	grain
	fish
	tabacco
	iron
	copper
	cloth
	ivory
	slaves
	salt
	wool
	fur
	gold
	sugar
	naval_supplies
	tea
	coffee
	spices
	wine
	cocoa
	silk
	dyes
	tropical_wood
	*/

	double tradeGoodsPrice = 0;

	if (tradeGoods == "chinaware")
	{
		tradeGoodsPrice = 3;
	}
	else if (tradeGoods == "grain")
	{
		tradeGoodsPrice = 2;
	}
	else if (tradeGoods == "fish")
	{
		tradeGoodsPrice = 2.5;
	}
	else if (tradeGoods == "tabacco")
	{
		tradeGoodsPrice = 3;
	}
	else if (tradeGoods == "iron")
	{
		tradeGoodsPrice = 3;
	}
	else if (tradeGoods == "copper")
	{
		tradeGoodsPrice = 3;
	}
	else if (tradeGoods == "cloth")
	{
		tradeGoodsPrice = 3;
	}
	else if (tradeGoods == "slaves")
	{
		tradeGoodsPrice = 2;
	}
	else if (tradeGoods == "salt")
	{
		tradeGoodsPrice = 3;
	}
	else if (tradeGoods == "gold")
	{
		tradeGoodsPrice = 6;
	}
	else if (tradeGoods == "fur")
	{
		tradeGoodsPrice = 2;
	}
	else if (tradeGoods == "sugar")
	{
		tradeGoodsPrice = 3;
	}
	else if (tradeGoods == "naval_supplies")
	{
		tradeGoodsPrice = 2;
	}
	else if (tradeGoods == "tea")
	{
		tradeGoodsPrice = 2;
	}
	else if (tradeGoods == "coffee")
	{
		tradeGoodsPrice = 3;
	}
	else if (tradeGoods == "spices")
	{
		tradeGoodsPrice = 3;
	}
	else if (tradeGoods == "wine")
	{
		tradeGoodsPrice = 2.5;
	}
	else if (tradeGoods == "cocoa")
	{
		tradeGoodsPrice = 4;
	}
	else if (tradeGoods == "ivory")
	{
		tradeGoodsPrice = 4;
	}
	else if (tradeGoods == "wool")
	{
		tradeGoodsPrice = 2.5;
	}
	else if (tradeGoods == "cotton")
	{
		tradeGoodsPrice = 3;
	}
	else if (tradeGoods == "dyes")
	{
		tradeGoodsPrice = 4;
	}
	else if (tradeGoods == "tropical_wood")
	{
		tradeGoodsPrice = 2;
	}
	else if (tradeGoods == "silk")
	{
		tradeGoodsPrice = 4;
	}
	else
	{
		// anything ive missed
		tradeGoodsPrice = 1;
	}

	return tradeGoodsPrice;
}


EU4::BuildingWeightEffects EU4::Province::getProvBuildingWeight(
	const Buildings& buildingTypes,
	const Modifiers& modifierTypes
) const
{
	BuildingWeightEffects effects;

	for (auto buildingName : buildings.getBuildings())
	{
		auto theBuilding = buildingTypes.getBuilding(buildingName);
		if (theBuilding)
		{
			effects.buildingWeight += theBuilding->getCost() * BUILDING_COST_TO_WEIGHT_RATIO;
			if (theBuilding->isManufactory())
			{
				effects.manufactoriesValue += 1.0;
			}
			for (auto effect: theBuilding->getModifier().getAllEffects())
			{
				if (effect.first == "local_manpower_modifier")
				{
					effects.manpowerModifier += effect.second;
				}
				else if (effect.first == "local_tax_modifier")
				{
					effects.taxModifier += effect.second;
				}
				else if (effect.first == "local_production_efficiency")
				{
					effects.productionEfficiency += effect.second;
				}
				else if (effect.first == "province_trade_power_modifier")
				{
					effects.tradePower += effect.second;
				}
				else if (effect.first == "trade_efficiency")
				{
					effects.tradeEfficiency += effect.second;
				}
				else if (effect.first == "trade_goods_size")
				{
					effects.tradeGoodsSizeModifier += effect.second;
				}
				else if (effect.first == "trade_goods_size_modifier")
				{
					effects.tradeGoodsSizeModifier += effect.second;
				}
				else if (effect.first == "trade_value_modifier")
				{
					effects.tradeValue += effect.second;
				}
				else if (effect.first == "trade_steering")
				{
					effects.tradeSteering += effect.second;
				}
			}
		}
		else
		{
			LOG(LogLevel::Warning) << "Could not look up information for building type " << buildingName;
		}
	}

	for (auto modifierName: modifiers)
	{
		auto theModifier = modifierTypes.getModifier(modifierName);
		if (theModifier)
		{
			for (auto effect : theModifier->getAllEffects())
			{
				if (effect.first == "local_manpower_modifier")
				{
					effects.manpowerModifier += effect.second;
				}
				else if (effect.first == "local_tax_modifier")
				{
					effects.taxModifier += effect.second;
				}
				else if (effect.first == "local_production_efficiency")
				{
					effects.productionEfficiency += effect.second;
				}
				else if (effect.first == "province_trade_power_modifier")
				{
					effects.tradePower += effect.second;
				}
				else if (effect.first == "trade_efficiency")
				{
					effects.tradeEfficiency += effect.second;
				}
				else if (effect.first == "trade_goods_size")
				{
					effects.tradeGoodsSizeModifier += effect.second;
				}
				else if (effect.first == "trade_goods_size_modifier")
				{
					effects.tradeGoodsSizeModifier += effect.second;
				}
				else if (effect.first == "trade_value_modifier")
				{
					effects.tradeValue += effect.second;
				}
				else if (effect.first == "trade_steering")
				{
					effects.tradeSteering += effect.second;
				}
			}
		}
		else
		{
			LOG(LogLevel::Warning) << "Could not look up information for modifier type " << modifierName;
		}
	}

	if (centerOfTradeLevel == 1)
	{
		effects.tradePower += 5;
	}
	else if (centerOfTradeLevel == 2)
	{
		effects.tradePower += 10;
	}
	else if (centerOfTradeLevel == 3)
	{
		effects.tradePower += 25;
	}

	return effects;
}
