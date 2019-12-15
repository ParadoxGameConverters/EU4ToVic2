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



#ifndef EU4PROVINCE_H_
#define EU4PROVINCE_H_



#include "Date.h"
#include "BuildingWeightEffects.h"
#include "GreatProjects.h"
#include "PopRatio.h"
#include "ProvinceBuildings.h"
#include "ProvinceHistory.h"
#include "ProvinceStats.h"
#include "../Buildings/Buildings.h"
#include "../Modifiers/Modifiers.h"
#include "newParser.h"
#include <string>
#include <vector>
#include <map>
#include <memory>



namespace EU4
{

class Country;
class Religions;


class Province: commonItems::parser
{
	public:
		Province(
			const std::string& numString,
			std::istream& theStream,
			const Buildings& buildingTypes,
			const Modifiers& modifierTypes
		);

		void addCore(const std::string& tag) { cores.insert(tag); }
		void removeCore(const std::string& tag) { cores.erase(tag); }

		bool wasInfidelConquest(const std::string& ownerReligion, const EU4::Religions& allReligions) const;
		bool hasBuilding(const std::string& building) const;
		bool hasGreatProject(const std::string& greatProject) const;
		double getCulturePercent(const std::string& culture) const;

		std::string getArea() const { return areaName; }
		int getNum() const { return num; }
		std::string getName() const { return name; }
		std::string getOwnerString() const { return ownerString; }
		std::string getControllerString() const { return controllerString; }
		std::set<std::string> getCores() const { return cores; }
		bool inHre() const { return inHRE; }
        bool isTerritorialCore() const { return territorialCore; }
        bool isColony() const { return colony; }
	        bool isState() const { return stated; }
	        bool wasColonised() const { return hadOriginalColoniser || provinceHistory->wasColonized(); }
		bool hasModifier(const std::string& modifierName) const { return modifiers.count(modifierName) > 0; }
		std::vector<EU4::PopRatio> getPopRatios() const { return provinceHistory->getPopRatios(); }
		std::optional<date> getFirstOwnedDate() const { return provinceHistory->getFirstOwnedDate(); }

		// getters for weight attributes
		double getTotalWeight() const { return totalWeight; }
		double getBaseTax() const { return baseTax; }
		double getTaxIncome() const { return taxIncome; }
		double getProductionIncome() const { return productionIncome; }
		double getManpowerWeight() const { return manpowerWeight; }
		double getTotalBuildingWeight() const { return buildingWeight; }
		double getTotalDevModifier() const { return devModifier; }
		ProvinceStats getProvinceStats() const { return provinceStats; }
		std::string getTradeGoods() const { return tradeGoods; }
	        double getProsperity() const { return prosperity; }

	        void setArea(const std::string& a) { areaName = a; }
	        void makeState(double p);

	      private:
		void determineProvinceWeight(const Buildings& buildingTypes, const Modifiers& modifierTypes);
		BuildingWeightEffects getProvBuildingWeight(const Buildings& buildingTypes, const Modifiers& modifierTypes) const;
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

		std::unique_ptr<EU4::ProvinceHistory> provinceHistory;
		std::unique_ptr<EU4::ProvinceBuildings> buildings;
		std::unique_ptr<EU4::GreatProjects> greatProjects;
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
		int centerOfTradeLevel = 0;
		ProvinceStats provinceStats;
	        double prosperity = 0;
	        bool stated = false;
};

}



#endif // EU4PROVINCE_H_
