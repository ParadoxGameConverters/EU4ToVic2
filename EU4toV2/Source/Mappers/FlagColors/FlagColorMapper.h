#ifndef FLAG_COLOR_MAPPER_H
#define FLAG_COLOR_MAPPER_H

#include "Parser.h"
#include "newColor.h"

namespace mappers
{
class FlagColorMapper: commonItems::parser
{
  public:
	FlagColorMapper();
	explicit FlagColorMapper(std::istream& theStream);

	[[nodiscard]] auto getNumFlagColors() const { return flagColorMapping.size(); }
	[[nodiscard]] std::optional<commonItems::newColor> getFlagColorByIndex(int index) const;

  private:
	void registerKeys();

	std::vector<commonItems::newColor> flagColorMapping;
};
}; // namespace mappers

#endif // FLAG_COLOR_MAPPER_H
