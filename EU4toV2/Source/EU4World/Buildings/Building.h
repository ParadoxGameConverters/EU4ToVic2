#ifndef BUILDING_H
#define BUILDING_H

#include "../Modifiers/Modifier.h"
#include "newParser.h"

namespace EU4
{
	class Building: commonItems::parser
	{
	public:
		Building(std::istream& theStream);

		double getCost() const { return cost; }
		const Modifier& getModifier() const { return modifier; }
		bool isManufactory() { return manufactory; }

	private:
		double cost = 0.0;
		bool manufactory = false;
		Modifier modifier;
	};
}

#endif // BUILDING_H