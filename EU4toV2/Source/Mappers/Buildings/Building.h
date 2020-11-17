#ifndef BUILDING_H
#define BUILDING_H
#include "Modifiers/Modifier.h"
#include "Parser.h"

namespace mappers
{
class Building: commonItems::parser
{
  public:
	explicit Building(std::istream& theStream);

	[[nodiscard]] auto getCost() const { return cost; }
	[[nodiscard]] auto isManufactory() const { return manufactory; }
	[[nodiscard]] const auto& getModifier() const { return modifier; }

  private:
	void registerKeys();

	double cost = 0;
	bool manufactory = false;
	EU4::Modifier modifier;
};
} // namespace mappers

#endif // BUILDING_H