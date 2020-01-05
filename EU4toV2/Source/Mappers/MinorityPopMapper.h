#ifndef MINORITY_POP_MAPPER_H
#define MINORITY_POP_MAPPER_H

#include <vector>
#include "newParser.h"


class V2Pop;

namespace mappers
{
	class MinorityPopMapper : commonItems::parser
	{
	public:
		MinorityPopMapper();
		bool matchMinorityPop(V2Pop& pop) const;

	private:
		std::vector<std::pair<std::string, std::string>> minorityPopMap;

	};

};

#endif // MINORITY_POP_MAPPER_H
