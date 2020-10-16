#ifndef DECISIONS_H
#define DECISIONS_H

#include "Decision.h"
#include "Parser.h"

namespace V2
{
class Country;

class Decisions: commonItems::parser
{
  public:
	Decisions() = default;
	explicit Decisions(const std::string& filename);
	explicit Decisions(std::istream& theStream);

	void registerKeys();
	void updateDecisions(const std::map<std::string, std::shared_ptr<Country>>& countries);

	[[nodiscard]] const auto& getDecisions() const { return decisions; }
	
	friend std::ostream& operator<<(std::ostream& output, const Decisions& decisions);

  private:
	std::map<std::string, V2::Decision> decisions;
};
} // namespace V2


#endif // DECISIONS_H