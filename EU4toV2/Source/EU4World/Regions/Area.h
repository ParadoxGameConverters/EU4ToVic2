#ifndef EU4_AREA_H
#define EU4_AREA_H

#include "Color.h"
#include "newParser.h"
#include <set>

namespace EU4
{
	class Area : commonItems::parser
	{
	public:
		explicit Area(std::istream& theStream);
		
		[[nodiscard]] const auto& getProvinces() const { return provinces; }

	private:
		std::set<int> provinces;
	};
}

#endif // EU4_AREA_H