#ifndef EU4_AREAS_H
#define EU4_AREAS_H

#include "Area.h"
#include "Color.h"
#include "newParser.h"
#include <istream>
#include <map>
#include <set>
#include <string>

namespace EU4
{
	class Areas: commonItems::parser
	{
	public:
		Areas(std::istream& filename);
		const std::set<int> getProvincesInArea(const std::string& area) const;
		const std::map<std::string, std::set<int>>& getAreas() const { return theAreas; }

	private:
		std::map<std::string, std::set<int>> theAreas;
	};
}

#endif // EU4_AREAS_H