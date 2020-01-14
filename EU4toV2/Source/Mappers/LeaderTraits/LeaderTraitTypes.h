#ifndef LEADER_TRAIT_TYPES_H
#define LEADER_TRAIT_TYPES_H

#include "newParser.h"
#include "LeaderTrait.h"
#include <vector>

namespace mappers
{
	class LeaderTraitTypes : commonItems::parser
	{
	public:
		LeaderTraitTypes(std::istream& theStream);
		std::vector<std::pair<std::string, LeaderTrait>> getTraits() const { return traits; }

	private:
		std::vector<std::pair<std::string, LeaderTrait>> traits;
	};
}

#endif // LEADER_TRAIT_TYPES_H