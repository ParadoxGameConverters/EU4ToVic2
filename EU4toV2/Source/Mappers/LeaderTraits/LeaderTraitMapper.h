#ifndef LEADER_TRAIT_MAPPER_H
#define LEADER_TRAIT_MAPPER_H

#include "newParser.h"
#include "LeaderTrait.h"


namespace mappers
{
	class LeaderTraitMapper : commonItems::parser
	{
	public:
		LeaderTraitMapper();
		LeaderTraitMapper(std::istream& theStream);
		std::optional<std::string> getPersonality(const int& fire, const int& shock, const int& maneuver, const int& siege) const;
		std::optional<std::string> getBackground(const int& fire, const int& shock, const int& maneuver, const int& siege) const;

	private:
		void registerKeys();
		std::vector<std::pair<std::string, LeaderTrait>> personalities;
		std::vector<std::pair<std::string, LeaderTrait>> backgrounds;
	};
}

#endif // LEADER_TRAIT_MAPPER_H