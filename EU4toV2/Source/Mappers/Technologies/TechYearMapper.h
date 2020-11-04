#ifndef TECH_YEAR_MAPPER_H
#define TECH_YEAR_MAPPER_H

#include "Parser.h"

namespace mappers
{

class TechYearMapper: commonItems::parser
{
  public:
	TechYearMapper();

	[[nodiscard]] const auto& getMap() const { return techYearMap; }

  private:
	void registerKeys();

	std::map<std::string, std::string> techYearMap;
};

} // namespace mappers

#endif // TECH_YEAR_MAPPER_H