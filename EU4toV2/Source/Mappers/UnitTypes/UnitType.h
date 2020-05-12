#ifndef UNIT_TYPE_H
#define UNIT_TYPE_H

#include "Parser.h"
#include "../../EU4World/Army/EU4Regiment.h"

namespace mappers
{
	class UnitType : commonItems::parser
	{
	public:
		UnitType() = default;
		explicit UnitType(std::istream& theStream);
		explicit UnitType(const std::string& filePath);
		
		void setStrength(int _strength) { strength = _strength; }
		
		[[nodiscard]] const auto& getName() const { return name; }
		[[nodiscard]] auto getCategory() const { return unitType; }
		[[nodiscard]] auto getStrength() const { return strength; }

	private:
		void registerKeys();
		
		std::string name;
		EU4::REGIMENTCATEGORY unitType = EU4::REGIMENTCATEGORY::num_reg_categories;
		int strength = 0;
	};
};

#endif // UNIT_TYPE_H
