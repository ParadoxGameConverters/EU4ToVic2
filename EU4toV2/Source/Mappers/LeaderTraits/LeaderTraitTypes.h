#ifndef LEADER_TRAIT_TYPES_H
#define LEADER_TRAIT_TYPES_H

#include "Parser.h"
#include "LeaderTrait.h"
#include <vector>

namespace mappers
{
	class LeaderTraitTypes : commonItems::parser
	{
	public:
		explicit LeaderTraitTypes(std::istream& theStream);
		
		[[nodiscard]] const auto& getTraits() const { return traits; }

	private:
		std::vector<std::pair<std::string, LeaderTrait>> traits;
	};
}

#endif // LEADER_TRAIT_TYPES_H