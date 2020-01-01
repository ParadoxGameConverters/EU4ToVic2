#ifndef UNIT_TYPE_H_
#define UNIT_TYPE_H_

#include "newParser.h"
#include "../EU4World/Army/EU4Regiment.h"


namespace mappers
{
	class UnitType : commonItems::parser
	{
	public:
		UnitType() = default;
		UnitType(std::istream& theStream);
		std::string getName() const { return name; }
		EU4::REGIMENTCATEGORY getCategory() const { return unitType; }
		void setStrength(int stren)  { strength = stren; }
		int getStrength() const { return strength; }

	private:
		std::string name;
		EU4::REGIMENTCATEGORY unitType = EU4::REGIMENTCATEGORY::num_reg_categories;
		int strength = 0;
	};
};

#endif // UNIT_TYPE_H_
