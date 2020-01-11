#ifndef BUILDING_H
#define BUILDING_H

#include "../../EU4World/Modifiers/Modifier.h"
#include "newParser.h"

namespace mappers
{
	class Building: commonItems::parser
	{
	public:
		Building(std::istream& theStream);

		double getCost() const { return cost; }
		const EU4::Modifier& getModifier() const { return modifier; }
		bool isManufactory() { return manufactory; }

	private:
		double cost = 0.0;
		bool manufactory = false;
		EU4::Modifier modifier;
	};
}

#endif // BUILDING_H