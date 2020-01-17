#ifndef MINORITY_POP_MAPPER_H
#define MINORITY_POP_MAPPER_H

#include "newParser.h"
#include "../../V2World/Pop/Pop.h"

namespace mappers
{
	class MinorityPopMapper : commonItems::parser
	{
	public:
		MinorityPopMapper();
		MinorityPopMapper(std::istream& theStream);
		bool matchMinorityPop(std::shared_ptr<V2::Pop> pop) const;

	private:
		void registerKeys();
		std::vector<std::pair<std::string, std::string>> minorityPopMap;

	};
};

#endif // MINORITY_POP_MAPPER_H
