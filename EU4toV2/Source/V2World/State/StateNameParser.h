#ifndef STATE_NAME_PARSER_H
#define STATE_NAME_PARSER_H
#include <map>
#include <optional>
#include <string>

namespace V2
{
class StateNameParser
{
  public:
	StateNameParser();
	explicit StateNameParser(std::string folder);

	[[nodiscard]] std::optional<std::string> getStateName(const std::string& stateID) const;
	[[nodiscard]] const auto& getStateNames() const { return stateNames; }

  private:
	std::map<std::string, std::string> stateNames;

	void importStateLocalizations(const std::string& file);
};
} // namespace V2
#endif // STATE_NAME_PARSER_H
