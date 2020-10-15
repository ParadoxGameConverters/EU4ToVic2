#ifndef DECISIONS_H
#define DECISIONS_H

#include "Decision.h"
#include "Parser.h"

namespace V2
{
class Decisions: commonItems::parser
{
  public:
	Decisions();
	Decisions(std::istream& theStream);

	void registerKeys();

	[[nodiscard]] const auto& getDecisions() const { return decisions; }

  private:
	std::map<std::string, V2::Decision> decisions;
};
} // namespace V2


#endif // DECISIONS_H