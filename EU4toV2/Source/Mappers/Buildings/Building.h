#ifndef BUILDING_H
#define BUILDING_H

#include "../../EU4World/Modifiers/Modifier.h"
#include "newParser.h"

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
		double cost = 0;
		bool manufactory = false;
		EU4::Modifier modifier;
	};
}

#endif // BUILDING_H