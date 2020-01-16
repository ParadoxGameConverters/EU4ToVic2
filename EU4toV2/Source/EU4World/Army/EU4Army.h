#ifndef EU4_ARMY_H
#define EU4_ARMY_H

#include <vector>
#include "EU4Regiment.h"
#include "../../Mappers/UnitTypes/UnitTypeMapper.h"

namespace EU4
{
	class EU4Army : public commonItems::parser
	{
	public:
		EU4Army() = default;
		EU4Army(std::istream& theStream, const std::string& _armyFloats); // Also applies to ships
		std::string getName() const { return name; }
		int getLocation() const { return location; }
		int getAtSea() const { return atSea; }
		int getId() const { return armyId; }
		int getLeaderId() const { return leaderId; }
		bool getArmyFloats() const { return armyFloats; }
		const std::vector<EU4Regiment>& getRegiments() const { return regimentList; }
		
		double getAverageStrength(REGIMENTCATEGORY category) const;
		void resolveRegimentTypes(const mappers::UnitTypeMapper& utm);
		int getTotalTypeStrength(REGIMENTCATEGORY category) const;

	private:
		std::string name;
		int location = -1;
		int atSea = 0; // obsolete since 1.20
		int armyId = 0;
		int leaderId = 0;
		bool armyFloats = false;
		std::vector<EU4Regiment> regimentList;
	};
}
#endif // EU4_ARMY_H
