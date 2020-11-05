#ifndef STARTING_TECH_H
#define STARTING_TECH_H

#include "Parser.h"

namespace mappers
{
class StartingTech: commonItems::parser
{
  public:
	explicit StartingTech(std::istream& theStream);

	std::string getName(const std::map<std::string, std::string>& idNameMap) const;

	[[nodiscard]] const auto& getId() const { return id; }
	[[nodiscard]] const auto& getGroup() const { return group; }
	[[nodiscard]] auto getScore() const { return score; }

  private:
	std::string name;
	std::string id;
	std::string group;
	double score = -1;
};
} // namespace mappers

#endif // STARTING_TECH_H