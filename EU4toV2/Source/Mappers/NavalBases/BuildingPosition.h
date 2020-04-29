#ifndef BUILDING_POSITION_H
#define BUILDING_POSITION_H

#include "Parser.h"

namespace mappers
{
	class BuildingPosition: commonItems::parser
	{
	public:
		BuildingPosition() = default;
		explicit BuildingPosition(std::istream& theStream);
		
		[[nodiscard]] auto getFound() const { return found; }

	private:
		bool found = false;
	};
}

#endif // BUILDING_POSITION_H
