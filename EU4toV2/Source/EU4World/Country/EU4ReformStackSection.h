#ifndef EU4_REFORM_STACK_SECTION_H
#define EU4_REFORM_STACK_SECTION_H

#include <set>
#include "Parser.h"

namespace EU4
{
	class ReformStackSection : commonItems::parser
	{
	public:
		explicit ReformStackSection(std::istream& theStream);
		[[nodiscard]] const auto& getReforms() const { return reforms; }

	private:
		std::set<std::string> reforms;
	};
}

#endif // EU4_REFORM_STACK_SECTION_H
