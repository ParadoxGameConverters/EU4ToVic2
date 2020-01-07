#ifndef EU4_REFORMS_SECTION_H
#define EU4_REFORMS_SECTION_H

#include "newParser.h"
#include <set>

namespace EU4
{
	class ReformsSection : commonItems::parser
	{
	public:
		ReformsSection(std::istream& theStream);
		const std::set<std::string>& getReforms() const { return reforms; }

	private:
		std::set<std::string> reforms;
	};
}

#endif // EU4_REFORMSSECTION_H
