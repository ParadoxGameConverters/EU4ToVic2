#ifndef EU4_REGIMENT_H_
#define EU4_REGIMENT_H_

#include "EU4UnitID.h"
#include <map>

namespace EU4
{
	enum class REGIMENTCATEGORY 
	{
		// ground units
		infantry,
		cavalry,
		artillery,
		// navy units
		heavy_ship,
		light_ship,
		galley,
		transport,
		// end of enum
		num_reg_categories
	};

	static std::map<std::string, REGIMENTCATEGORY> RegimentCategoryNames
	{
		{ "infantry", REGIMENTCATEGORY::infantry },
		{ "cavalry", REGIMENTCATEGORY::cavalry },
		{ "artillery", REGIMENTCATEGORY::artillery },
		{ "heavy_ship", REGIMENTCATEGORY::heavy_ship },
		{ "light_ship", REGIMENTCATEGORY::light_ship },
		{ "galley", REGIMENTCATEGORY::galley },
		{ "transport", REGIMENTCATEGORY::transport }
	};

	static std::map<REGIMENTCATEGORY, std::string> RegimentCategoryTypes
	{
		{ REGIMENTCATEGORY::infantry, "infantry" },
		{ REGIMENTCATEGORY::cavalry, "cavalry" },
		{ REGIMENTCATEGORY::artillery, "artillery" },
		{ REGIMENTCATEGORY::heavy_ship, "heavy_ship" },
		{ REGIMENTCATEGORY::light_ship, "light_ship" },
		{ REGIMENTCATEGORY::galley, "galley" },
		{ REGIMENTCATEGORY::transport, "transport" }
	};

	class EU4Regiment : public  commonItems::parser
	{
	public:
		EU4Regiment() = default;
		EU4Regiment(std::istream& theStream); // Also applies to ships
		std::string getType() const { return regimentType; }
		std::string getName() const { return name; }
		int getHome() const { return home; }
		REGIMENTCATEGORY getCategory() const { return category; }
		double getMorale() const { return morale; }
		double getStrength() const { return strength; }
		int getTypeStrength() const { return typeStrength; }
		EU4UnitID getId() const { return regimentId; }
		void setCategory(const REGIMENTCATEGORY cat) { category = cat; }
		void setTypeStrength(const int tStrength) { typeStrength = tStrength; }

	private:
		std::string name;
		std::string regimentType;
		int home = 0;
		int typeStrength = 0;
		REGIMENTCATEGORY category = REGIMENTCATEGORY::num_reg_categories;
		double morale = 0.0;
		double strength = 0.0;
		EU4UnitID regimentId;
	};
}

#endif // EU4_REGIMENT_H_