#ifndef BUILDING_POSITION_H
#define BUILDING_POSITION_H

#include "newParser.h"

namespace mappers
{
	class BuildingPosition: commonItems::parser
	{
	public:
		BuildingPosition() = default;
		BuildingPosition(std::istream& theStream);
		bool getFound() const { return found; }

	private:
		bool found = false;
	};
}

#endif // BUILDING_POSITION_H
