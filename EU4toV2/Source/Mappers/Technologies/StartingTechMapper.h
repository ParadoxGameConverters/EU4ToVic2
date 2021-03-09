#ifndef STARTING_TECH_MAPPER_H
#define STARTING_TECH_MAPPER_H

#include "Parser.h"
#include "StartingTech.h"

namespace mappers
{
class Technologies;

class StartingTechMapper: commonItems::parser
{
  public:
	StartingTechMapper();

	[[nodiscard]] std::vector<std::string> getTechsForGroupAndScore(const std::string& group,
		 double score,
		 const std::map<std::string, std::string>& idNameMap) const;

  private:
	std::vector<StartingTech> startingTechs;
};
} // namespace mappers

#endif // STARTING_TECH_MAPPER_H
