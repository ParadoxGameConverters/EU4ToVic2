#ifndef STATE_MAPPER_H
#define STATE_MAPPER_H

#include "Parser.h"
#include <map>
#include <set>

namespace mappers
{
	class StateMapper: commonItems::parser
	{
	public:
		StateMapper();
		explicit StateMapper(std::istream& theStream);
		explicit StateMapper(std::string& filename);
		
		[[nodiscard]] std::set<int> getAllProvincesInState(int province) const;
		[[nodiscard]] const auto& getStateMap() const { return stateMap; }

	private:
		void registerKeys();
		std::map<int, std::set<int>> stateProvincesMap;
		std::map<int, std::set<int>> stateMap;
	};
}

#endif // STATE_MAPPER_H