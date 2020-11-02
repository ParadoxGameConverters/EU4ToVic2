#ifndef EU4_LEADER_ID_H
#define EU4_LEADER_ID_H
#include "Parser.h"

namespace EU4
{
class LeaderID: commonItems::parser
{
  public:
	LeaderID() = default;
	explicit LeaderID(std::istream& theStream);

	[[nodiscard]] const auto& getIDNum() const { return IDNum; }

  private:
	void registerKeys();

	std::optional<int> IDNum;
};
} // namespace EU4

#endif // EU4_LEADER_ID_H