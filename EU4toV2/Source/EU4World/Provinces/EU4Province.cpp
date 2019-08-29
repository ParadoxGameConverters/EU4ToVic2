/*Copyright(c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */



#include "EU4Province.h"
#include "../EU4Country.h"
#include "../Religions/Religions.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "../../Configuration.h"
#include <algorithm>
#include <fstream>
#include <optional>
#include <sstream>



const double BUILDING_COST_TO_WEIGHT_RATIO = 0.02;



EU4::Province::Province(const std::string& numString, std::istream& theStream, const Buildings& buildingTypes)
{
	registerKeyword(std::regex("name"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleString nameString(theStream);
		name = nameString.getString();
	});
	registerKeyword(std::regex("base_tax"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleDouble baseTaxDouble(theStream);
		baseTax = baseTaxDouble.getDouble();
	});
	registerKeyword(std::regex("base_production"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleDouble baseProductionDouble(theStream);
		baseProduction = baseProductionDouble.getDouble();
	});
	registerKeyword(std::regex("base_manpower"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleDouble manpowerDouble(theStream);
		manpower = manpowerDouble.getDouble();
	});
	registerKeyword(std::regex("manpower"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleDouble manpowerDouble(theStream);
		manpower = manpowerDouble.getDouble();
	});
	registerKeyword(std::regex("owner"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleString ownerStringString(theStream);
		ownerString = ownerStringString.getString();
	});
	registerKeyword(std::regex("controller"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleString controllerStringString(theStream);
		controllerString = controllerStringString.getString();
	});
	registerKeyword(std::regex("cores"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::stringList coresStrings(theStream);
		for (auto coreString : coresStrings.getStrings())
		{
			cores.insert(coreString);
		}
	});
	registerKeyword(std::regex("core"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleString coresString(theStream);
		cores.insert(coresString.getString());
	});
	registerKeyword(std::regex("hre"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleString hreString(theStream);
		if (hreString.getString() == "yes")
		{
			inHRE = true;
		}
	});
	registerKeyword(std::regex("colonysize"), [this](const std::string & unused, std::istream & theStream) {
		commonItems::ignoreItem(unused, theStream);
		colony = true;
	});
	registerKeyword(std::regex("original_coloniser"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::ignoreItem(unused, theStream);
		hadOriginalColoniser = true;
	});
	registerKeyword(std::regex("history"), [this](const std::string& unused, std::istream& theStream) {
		provinceHistory = std::make_unique<ProvinceHistory>(theStream);
	});
	registerKeyword(std::regex("buildings"), [this](const std::string& unused, std::istream& theStream) {
		buildings = std::make_unique<ProvinceBuildings>(theStream);
	});
	registerKeyword(std::regex("great_projects"), [this](const std::string& unused, std::istream& theStream) {
		greatProjects = std::make_unique<GreatProjects>(theStream);
	});
	registerKeyword(std::regex("trade_goods"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleString tradeGoodsString(theStream);
		tradeGoods = tradeGoodsString.getString();
	});
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), commonItems::ignoreItem);

	parseStream(theStream);

	num = 0 - stoi(numString);

	// for old versions of EU4 (< 1.12), copy tax to production if necessary
	if ((baseProduction == 0.0f) && (baseTax > 0.0f))
	{
		baseProduction = baseTax;
	}
	if (!provinceHistory)
	{
		std::stringstream input;
		provinceHistory = std::make_unique<ProvinceHistory>(input);
	}

	determineProvinceWeight(buildingTypes);
}


bool EU4::Province::wasInfidelConquest(const std::string& ownerReligion, const EU4::Religions& allReligions) const
{
	return provinceHistory->wasInfidelConquest(allReligions, ownerReligion, num);
}


bool EU4::Province::hasBuilding(const std::string& building) const
{
	if (buildings && buildings->hasBuilding(building))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool EU4::Province::hasGreatProject(const std::string& greatProject) const
{
	if (greatProjects && greatProjects->hasGreatProject(greatProject))
	{
		return true;
	}
	else
	{
		return false;
	}
}


double EU4::Province::getCulturePercent(const std::string& culture) const
{
	double culturePercent = 0.0f;

	for (auto pop: provinceHistory->getPopRatios())
	{
		if (pop.getCulture() == culture)
		{
			culturePercent += pop.getLowerRatio();
		}
	}

	return culturePercent;
}


void EU4::Province::determineProvinceWeight(const Buildings& buildingTypes)
{
	double trade_goods_weight			= getTradeGoodWeight();
	double manpower_weight				= manpower;
	double building_weight				= 0.0;
	double manpower_modifier			= 0.0;
	double manu_gp_mod					= 0.0;
	double building_tx_eff				= 0.0;
	double production_eff				= 0.0;
	double building_tx_income			= 0.0;
	double manpower_eff					= 0.0;
	double goods_produced_perc_mod	= 0.0;
	double trade_power					= 0.0;
	double trade_value					= 0.0;
	double trade_value_eff				= 0.0;
	double trade_power_eff				= 0.0;
	double dev_modifier				= 0.0;
	
	std::vector<double> provBuildingWeightVec = getProvBuildingWeight(buildingTypes);

	// 0 building_weight, 1 manpower_modifier, 2 manu_gp_mod, 3 building_tx_eff, 4 production_eff
	// 5 building_tx_income, 6 manpower_eff, 7 goods_produced_perc_mod, 8 trade_power 9 trade_value
	// 10 trade_value_eff, 11 trade_power_eff;
	try {
		building_weight			= provBuildingWeightVec.at(0);
		manpower_modifier			= provBuildingWeightVec.at(1);
		manu_gp_mod					= provBuildingWeightVec.at(2);
		building_tx_eff			= provBuildingWeightVec.at(3);
		production_eff				= provBuildingWeightVec.at(4);
		building_tx_income		= provBuildingWeightVec.at(5);
		manpower_eff				= provBuildingWeightVec.at(6);
		goods_produced_perc_mod	= provBuildingWeightVec.at(7);
		trade_power					= provBuildingWeightVec.at(8);
		trade_value					= provBuildingWeightVec.at(9);
		trade_value_eff			= provBuildingWeightVec.at(10);
		trade_power_eff			= provBuildingWeightVec.at(11);
		dev_modifier			= provBuildingWeightVec.at(12);
	}
	catch (exception &e)
	{
		LOG(LogLevel::Error) << "Error in building weight vector: " << e.what();
	}

	// Check tag, ex. TIB has goods_produced +0.05
	// This needs to be hard coded unless there's some other way of figuring out modded national ambitions/ideas
	if (ownerString == "TIB")
	{
		goods_produced_perc_mod += 0.05;
	}

	double goods_produced = (baseProduction * 0.2) + manu_gp_mod + goods_produced_perc_mod + 0.03;

	// idea effects
	/*if ( (owner !=  NULL) && (owner->hasNationalIdea("bureaucracy")) )
	{
		building_tx_eff += 0.10;
	}
	if ( (owner !=  NULL) && (owner->hasNationalIdea("smithian_economics")) )
	{
		production_eff += 0.10;
	}*/

	// manpower
	manpower_weight *= 25;
	manpower_weight += manpower_modifier;
	manpower_weight *= ((1 + manpower_modifier) / 25); // should work now as intended

	//LOG(LogLevel::Info) << "Manpower Weight: " << manpower_weight;

	double total_tx = (baseTax + building_tx_income) * (1.0 + building_tx_eff + 0.15);
	double production_eff_tech = 0.5; // used to be 1.0

	double total_trade_value = ((getTradeGoodPrice() * goods_produced) + trade_value) * (1 + trade_value_eff);
	double production_income = total_trade_value * (1 + production_eff_tech + production_eff);
	//LOG(LogLevel::Info) << "province name: " << this->getProvName() 
	//	<< " trade good: " << tradeGoods 
	//	<< " Price: " << getTradeGoodPrice() 
	//	<< " trade value: " << trade_value 
	//	<< " trade value eff: " 
	//	<< (1 + trade_value_eff) 
	//	<< " goods produced: " << goods_produced 
	//	<< " production eff: " << production_eff 
	//	<< " Production: " << production_income;

	total_tx *= 1.5;
	manpower_weight *= 1;
	production_income *= 1.5;

	buildingWeight = building_weight;
	taxIncome = total_tx;
	productionIncome = production_income;
	manpowerWeight = manpower_weight;
	tradeGoodWeight	= trade_goods_weight;
	devModifier	= dev_modifier;
	
	// dev modifier
	dev_modifier *= ( baseTax + baseProduction + manpower );

	totalWeight = building_weight + dev_modifier + ( manpower_weight + production_income + total_tx );
	//i would change dev effect to 1, but your choice
	if (ownerString == "")
	{
		totalWeight = 0;
	}

	provinceStats.setGoodsProduced(goods_produced);
	provinceStats.setPrice(getTradeGoodPrice());
	provinceStats.setTradeEfficiency(1 + trade_value_eff);
	provinceStats.setProductionEfficiency(1 + production_eff);
	provinceStats.setTradeValue(trade_value);
	provinceStats.setTradeValue(production_income);
	provinceStats.setBaseTax(baseTax);
	provinceStats.setBuildingsIncome(building_tx_income);
	provinceStats.setTaxEfficiency(1 + building_tx_eff);
	provinceStats.setTotalTaxIncome(total_tx);
	provinceStats.setTotalTradeValue(total_trade_value);
	//LOG(LogLevel::Info) << "Num: " << num << " TAG: " << ownerString << " Weight: " << totalWeight;
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
	//LOG(LogLevel::Info) << "Trade Goods Price";
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


double EU4::Province::getTradeGoodWeight() const
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
	*/

	double trade_goods_weight = 0;
	if (tradeGoods == "chinaware")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "grain")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "fish")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "tabacco")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "iron")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "copper")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "cloth")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "slaves")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "salt")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "gold")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "fur")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "sugar")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "naval_supplies")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "tea")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "coffee")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "spices")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "wine")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "cocoa")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "ivory")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "wool")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "cotton")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "silk")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "tropical_wood")
	{
		trade_goods_weight = 2;
	}
	else if (tradeGoods == "dyes")
	{
		trade_goods_weight = 2;
	}
	else
	{
		// anything ive missed
		trade_goods_weight = 0;
	}

	return trade_goods_weight;
}


std::vector<double> EU4::Province::getProvBuildingWeight(const Buildings& buildingTypes) const
{
	double buildingWeight = 0.0;
	double manufactoriesValue = 0.0;
	double manpowerModifier = 0.0;
	double taxModifier = 0.0;
	double productionEfficiency = 0.0;
	double tradePower = 0.0;
	double devModifier = 0.0;

	if (buildings)
	{
		for (auto buildingName : buildings->getBuildings())
		{
			auto theBuilding = buildingTypes.getBuilding(buildingName);
			if (theBuilding)
			{
				buildingWeight += theBuilding->getCost() * BUILDING_COST_TO_WEIGHT_RATIO;
				if (theBuilding->isManufactory())
				{
					manufactoriesValue += 1.0;
				}
				for (auto modifier : theBuilding->getBuildingModifiers().getAllModifiers())
				{
					if (modifier.first == "local_manpower_modifier")
					{
						manpowerModifier += modifier.second;
					}
					else if (modifier.first == "local_tax_modifier")
					{
						taxModifier += modifier.second;
					}
					else if (modifier.first == "local_production_efficiency")
					{
						productionEfficiency += modifier.second;
					}
					else if (modifier.first == "province_trade_power_modifier")
					{
						tradePower += modifier.second;
					}
				}
			}
			else
			{
				LOG(LogLevel::Warning) << "Could not look up information for building type " << buildingName;
			}
		}

		if (hasBuilding("center_of_trade"))
		{
			buildingWeight += 24;
		}

		if (hasBuilding("inland_center_of_trade"))
		{
			buildingWeight += 12;
		}

		if (hasBuilding("natural_harbor"))
		{
			buildingWeight += 15;
		}

		if (hasBuilding("stora_kopparberget_modifier"))
		{
			manufactoriesValue = 5.0;
		}

		if (hasBuilding("cerro_rico_modifier"))
		{
			manufactoriesValue = 3.0;
		}

		if (hasBuilding("spice_islands_modifier"))
		{
			manufactoriesValue = 3.0;
		}
	}

	std::vector<double> provBuildingWeightVec;
	provBuildingWeightVec.push_back(buildingWeight);
	provBuildingWeightVec.push_back(manpowerModifier);
	provBuildingWeightVec.push_back(manufactoriesValue);
	provBuildingWeightVec.push_back(0.0);
	provBuildingWeightVec.push_back(productionEfficiency);
	provBuildingWeightVec.push_back(taxIncome);
	provBuildingWeightVec.push_back(0.0);
	provBuildingWeightVec.push_back(0.0);
	provBuildingWeightVec.push_back(tradePower);
	provBuildingWeightVec.push_back(0.0);
	provBuildingWeightVec.push_back(0.0);
	provBuildingWeightVec.push_back(0.0);
	provBuildingWeightVec.push_back(0.0);
	// 0 building_weight, 1 manpower_modifier, 2 manu_gp_mod, 3 building_tx_eff, 4 production_eff
	// 5 building_tx_income, 6 manpower_eff, 7 goods_produced_perc_mod, 8 trade_power 9 trade_value
	// 10 trade_value_eff, 11 trade_power_eff;
	return provBuildingWeightVec;
}