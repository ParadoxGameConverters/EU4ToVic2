#ifndef EU4_COLONIAL_REGION_H
#define EU4_COLONIAL_REGION_H

#include "newParser.h"
#include <set>

namespace EU4
{
	class ColonialRegion : commonItems::parser
	{
	public:
		explicit ColonialRegion(std::istream& theStream);
		[[nodiscard]] const auto& getProvinces() const { return provinces; }

	private:
		std::set<int> provinces;
	};
}

#endif // EU4_COLONIAL_REGION_H