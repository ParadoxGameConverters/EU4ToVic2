#ifndef V2_LEADER_TRAIT_MAPPER_H_
#define V2_LEADER_TRAIT_MAPPER_H_

#include "newParser.h"
#include "V2LeaderTrait.h"
#include <map>


namespace mappers
{
	class V2LeaderTraitMapper : commonItems::parser
	{
	public:
		V2LeaderTraitMapper();
		std::optional<std::string> getPersonality(int fire, int shock, int manuever, int siege) const;
		std::optional<std::string> getBackground(int fire, int shock, int manuever, int siege) const;

	private:
		std::vector<std::pair<std::string, V2LeaderTrait>> personalities;
		std::vector<std::pair<std::string, V2LeaderTrait>> backgrounds;
	};
}

#endif // V2_LEADER_TRAIT_MAPPER_H_