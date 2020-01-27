#ifndef EU4_AREAS_H
#define EU4_AREAS_H

#include "Color.h"
#include "newParser.h"
#include <map>
#include <set>
#include <string>

namespace EU4
{
	class Areas: commonItems::parser
	{
	public:
		explicit Areas(std::istream& theStream);

		[[nodiscard]] std::optional<std::set<int>> getProvincesInArea(const std::string& area) const;
		[[nodiscard]] const auto& getAreas() const { return theAreas; }

	private:
		std::map<std::string, std::set<int>> theAreas;
	};
}

#endif // EU4_AREAS_H