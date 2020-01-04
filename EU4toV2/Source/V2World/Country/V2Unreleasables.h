#ifndef V2_UNRELEASABLES_H_
#define V2_UNRELEASABLES_H_

#include "newParser.h"
#include <set>

namespace mappers
{
	class V2Unreleasables : commonItems::parser
	{
	public:
		V2Unreleasables();
		std::set<std::string> getUnreleasables() const { return unreleasables; }

	private:
		std::set<std::string> unreleasables;
	};
}

#endif // V2_UNRELEASABLES_H_