#ifndef AREA_NAMES_H
#define AREA_NAMES_H

#include "newParser.h"
#include <set>
#include <string>

namespace EU4
{
	class AreaNames: commonItems::parser
	{
	public:
		AreaNames(std::istream& theStream);
		const std::set<std::string>& getNames() const { return names; }

	private:
		std::set<std::string> names;
	};
}

#endif // AREA_NAMES_H