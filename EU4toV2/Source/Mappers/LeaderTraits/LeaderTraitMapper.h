#ifndef LEADER_TRAIT_MAPPER_H
#define LEADER_TRAIT_MAPPER_H

#include "Parser.h"
#include "LeaderTrait.h"


namespace mappers
{
	class LeaderTraitMapper : commonItems::parser
	{
	public:
		LeaderTraitMapper();
		explicit LeaderTraitMapper(std::istream& theStream);
		
		[[nodiscard]] std::optional<std::string> getPersonality(int fire, int shock, int maneuver, int siege) const;
		[[nodiscard]] std::optional<std::string> getBackground(int fire, int shock, int maneuver, int siege) const;

	private:
		void registerKeys();
		
		std::vector<std::pair<std::string, LeaderTrait>> personalities;
		std::vector<std::pair<std::string, LeaderTrait>> backgrounds;
	};
}

#endif // LEADER_TRAIT_MAPPER_H