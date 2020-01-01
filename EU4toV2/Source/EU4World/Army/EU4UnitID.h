#ifndef EU4_UNIT_ID_H_
#define EU4_UNIT_ID_H_

#include "newParser.h"

namespace EU4
{
	class EU4UnitID : public  commonItems::parser
	{
	public:
		EU4UnitID() = default;
		EU4UnitID(std::istream& theStream);
		int getType() const { return unitType; }
		int getId() const { return unitId; }

	private:
		int unitId = 0;
		int unitType = 0;
	};
}

#endif // EU4_UNIT_ID_H_