#ifndef UNIT_TYPE_H
#define UNIT_TYPE_H

#include "newParser.h"
#include "../../EU4World/Army/EU4Regiment.h"

namespace mappers
{
	class UnitType : commonItems::parser
	{
	public:
		UnitType() = default;
		UnitType(std::istream& theStream);
		UnitType(const std::string& filePath);
		std::string getName() const { return name; }
		EU4::REGIMENTCATEGORY getCategory() const { return unitType; }
		void setStrength(int _strength)  { strength = _strength; }
		int getStrength() const { return strength; }

	private:
		void registerKeys();
		std::string name;
		EU4::REGIMENTCATEGORY unitType = EU4::REGIMENTCATEGORY::num_reg_categories;
		int strength = 0;
	};
};

#endif // UNIT_TYPE_H
