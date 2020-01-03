#ifndef EU4_ARMY_H_
#define EU4_ARMY_H_

#include <vector>
#include "EU4Regiment.h"
#include "../../Mappers/UnitTypeMapper.h"
#include "../ID.h"

namespace EU4
{
	class EU4Army : public  commonItems::parser
	{
	public:
		EU4Army() = default;
		EU4Army(std::istream& theStream); // Also applies to ships
		std::string getName() const { return name; }
		int getLocation() const { return location; }
		int getAtSea() const { return atSea; }
		int getId() const { return armyId; }
		int getLeaderId() const { return leaderId; }
		std::vector<EU4Regiment> getRegiments() const { return regimentList; }
		
		double getAverageStrength(REGIMENTCATEGORY category) const;
		void resolveRegimentTypes(mappers::RegimentTypeMap RTmap);
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
		std::vector<int> blocked_homes; // invalid homes for this army
	};
}
#endif // EU4_ARMY_H_