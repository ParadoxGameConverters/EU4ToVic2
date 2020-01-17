#ifndef ARMY_H
#define ARMY_H

#include "Regiment.h"
#include "../../EU4World/Army/EU4Army.h"
#include "../../Mappers/RegimentCosts/RegimentCostsMapper.h"
#include "../../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "../../Mappers/Adjacency/AdjacencyMapper.h"
#include "../../Mappers/PortProvinces/PortProvinces.h"
#include "../V2Province.h"

namespace V2
{
	enum class addRegimentToArmyResult {success, retry, fail};

	class Army // also Navy
	{
	public:
		Army(const EU4::EU4Army& eu4Army,
			const std::string& _tag,
			bool civilized, 
			const mappers::RegimentCostsMapper& regimentCostsMapper, 
			std::map<int, V2Province*> allProvinces,
			const mappers::ProvinceMapper& provinceMapper,
			const mappers::PortProvinces& portProvincesMapper, 
			std::map<REGIMENTTYPE, int>& unitNameCount,
			const std::string& localAdjective);

		std::map<REGIMENTTYPE, double>& getArmyRemainders() { return armyRemainders; }
		bool hasRegimentsOfType(REGIMENTTYPE chosenType) const;
		double getRegimentRemainder(REGIMENTTYPE chosenType) const;
		void addRegimentRemainder(REGIMENTTYPE chosenType, double value) { armyRemainders[chosenType] += value; }
		bool success() const { return created; }
		
		friend std::ostream& operator<<(std::ostream& output, const Army& army);		

		addRegimentToArmyResult addRegimentToArmy(
			REGIMENTTYPE chosenType,
			std::map<int, V2Province*> allProvinces,
			const mappers::ProvinceMapper& provinceMapper,
			const mappers::PortProvinces& portProvincesMapper,
			std::map<REGIMENTTYPE, int>& unitNameCount,
			const std::string& localAdjective);

		static std::vector<int> getPortProvinces(
			const std::vector<int>& locationCandidates,
			std::map<int, V2Province*> allProvinces,
			const mappers::PortProvinces& portProvincesMapper);

	private:
		REGIMENTTYPE pickCategory(EU4::REGIMENTCATEGORY incCategory, bool civilized);
		std::optional<int> getProbabilisticHomeProvince(REGIMENTTYPE chosenType) const;
		void blockHomeProvince(int blocked);
		static V2Province* pickRandomPortProvince(std::vector<int> homeCandidates, std::map<int, V2Province*> allProvinces);
		static bool provinceRegimentCapacityPredicate(V2Province* prov1, V2Province* prov2);
		static V2Province* getProvinceForExpeditionaryArmy(std::map<int, V2Province*> allProvinces, const std::string& tag);
		static std::string getRegimentName(REGIMENTTYPE chosenType, std::map<REGIMENTTYPE, int>& unitNameCount, const std::string& localisation);
		static int pickRandomProvinceID(std::vector<int> homeCandidates);
				
		std::string name;
		int location = 0;
		std::vector<Regiment> regiments;
		std::map<EU4::REGIMENTCATEGORY, int> regimentCounts;
		std::map<REGIMENTTYPE, int> buildOrder;
		std::map<REGIMENTTYPE, int> buildSuceeded;
		std::map<REGIMENTTYPE, double> armyRemainders;
		std::map<REGIMENTTYPE, std::vector<int>> eu4homeProvinces;
		bool isNavy = false;
		std::string tag;
		bool created = true;
	};
}

#endif // ARMY_H
