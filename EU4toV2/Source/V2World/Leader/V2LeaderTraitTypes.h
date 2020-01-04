#ifndef V2_LEADER_TRAIT_TYPES_H_
#define V2_LEADER_TRAIT_TYPES_H_

#include "newParser.h"
#include "V2LeaderTrait.h"
#include <vector>


namespace mappers
{
	class V2LeaderTraitTypes : commonItems::parser
	{
	public:
		V2LeaderTraitTypes(std::istream& theStream);
		std::vector<std::pair<std::string, V2LeaderTrait>> getTraits() const { return traits; }

	private:
		std::vector<std::pair<std::string, V2LeaderTrait>> traits;
	};
}

#endif // V2_LEADER_TRAIT_TYPES_H_