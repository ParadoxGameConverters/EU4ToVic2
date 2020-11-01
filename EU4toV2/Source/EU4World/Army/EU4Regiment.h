#ifndef EU4_REGIMENT_H
#define EU4_REGIMENT_H
#include "ID.h"
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

static std::map<std::string, REGIMENTCATEGORY> RegimentCategoryNames{{"infantry", REGIMENTCATEGORY::infantry},
	 {"cavalry", REGIMENTCATEGORY::cavalry},
	 {"artillery", REGIMENTCATEGORY::artillery},
	 {"heavy_ship", REGIMENTCATEGORY::heavy_ship},
	 {"light_ship", REGIMENTCATEGORY::light_ship},
	 {"galley", REGIMENTCATEGORY::galley},
	 {"transport", REGIMENTCATEGORY::transport}};

static std::map<REGIMENTCATEGORY, std::string> RegimentCategoryTypes{{REGIMENTCATEGORY::infantry, "infantry"},
	 {REGIMENTCATEGORY::cavalry, "cavalry"},
	 {REGIMENTCATEGORY::artillery, "artillery"},
	 {REGIMENTCATEGORY::heavy_ship, "heavy_ship"},
	 {REGIMENTCATEGORY::light_ship, "light_ship"},
	 {REGIMENTCATEGORY::galley, "galley"},
	 {REGIMENTCATEGORY::transport, "transport"},
	 {REGIMENTCATEGORY::num_reg_categories, "unassigned category!"}};

class EU4Regiment: public commonItems::parser
{
  public:
	EU4Regiment() = default;
	explicit EU4Regiment(std::istream& theStream); // Also applies to ships
	[[nodiscard]] const auto& getType() const { return regimentType; }
	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] auto getHome() const { return home; }
	[[nodiscard]] auto getCategory() const { return category; }
	[[nodiscard]] auto getTypeStrength() const { return typeStrength; }
	void setCategory(const REGIMENTCATEGORY cat) { category = cat; }
	void setTypeStrength(const int tStrength) { typeStrength = tStrength; }

  private:
	void registerKeys();

	std::string name;
	std::string regimentType;
	int home = 0;
	int typeStrength = 0;
	REGIMENTCATEGORY category = REGIMENTCATEGORY::num_reg_categories;
};
} // namespace EU4

#endif // EU4_REGIMENT_H