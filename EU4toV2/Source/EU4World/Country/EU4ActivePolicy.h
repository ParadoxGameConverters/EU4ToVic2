#ifndef EU4ACTIVE_POLICY_H
#define EU4ACTIVE_POLICY_H

#include "Parser.h"
#include <set>

namespace EU4
{
class EU4ActivePolicy: commonItems::parser
{
  public:
	explicit EU4ActivePolicy(std::istream& theStream);
	[[nodiscard]] const auto& getPolicy() const { return policy; }
	[[nodiscard]] const auto& getDate() const { return date; }

  private:
	std::string policy;
	std::string date;
};
} // namespace EU4

#endif // EU4ACTIVE_POLICY_H