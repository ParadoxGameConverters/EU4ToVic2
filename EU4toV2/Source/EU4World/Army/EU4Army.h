#ifndef EU4_ARMY_H
#define EU4_ARMY_H

#include <unordered_map>
#include <vector>
#include <set>
#include "EU4Regiment.h"
#include "../../Mappers/UnitTypes/UnitTypeMapper.h"
#include "../ID.h"

namespace EU4
{
	class EU4Army : public commonItems::parser
	{
	public:
		EU4Army() = default;
		EU4Army(std::istream& theStream); // Also applies to ships
		std::string getName() const { return name; }
		int getLocation() const { return location; }
		int getAtSea() const { return atSea; }
		int getId() const { return armyId; }
		int getLeaderId() const { return leaderId; }
		const std::vector<EU4Regiment>& getRegiments() const { return regimentList; }
		
		double getAverageStrength(REGIMENTCATEGORY category) const;
		void resolveRegimentTypes(const mappers::UnitTypeMapper& utm);
		int getTotalTypeStrength(REGIMENTCATEGORY category) const;
		std::optional<int> getProbabilisticHomeProvince(REGIMENTCATEGORY cat) const;
		void blockHomeProvince(const int homeId);

	private:
		std::string name;
		int location = -1;
		int atSea = 0; // obsolete since 1.20
		int armyId = 0;
		int leaderId = 0;
		std::vector<EU4Regiment> regimentList;
		// Unblocked home provinces; note that a province can be listed
		// multiple times.
		std::unordered_map<REGIMENTCATEGORY, std::vector<int>> home_provinces;
	};
}
#endif // EU4_ARMY_H
