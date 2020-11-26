#ifndef FACTORY_STARTING_COUNTS_H
#define FACTORY_STARTING_COUNTS_H
#include "Parser.h"

namespace mappers
{
class FactoryStartingCounts: commonItems::parser
{
  public:
	FactoryStartingCounts();
	explicit FactoryStartingCounts(std::istream& theStream);

	[[nodiscard]] const auto& getFactoryStartingCounts() const { return startingCounts; }

  private:
	void registerKeywords();

	std::map<std::string, int> startingCounts;
};
} // namespace mappers

#endif // FACTORY_STARTING_COUNTS_H