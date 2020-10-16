#ifndef DECISION_H
#define DECISION_H

#include <ostream>
#include "Parser.h"

namespace V2
{
class Decision: commonItems::parser
{
  public:
	Decision() = default;
	explicit Decision(std::istream& theStream);

	void registerKeys();
	void setTitle(const std::string& decisionTitle) { title = decisionTitle; }
	void updateDecision(const std::string& section, const std::string& newText);

	[[nodiscard]] const auto& getTitle() const { return title; }
	[[nodiscard]] const auto& getPicture() const { return picture; }
	[[nodiscard]] const auto& getNews() const { return news; }
	[[nodiscard]] const auto& getPotential() const { return potential; }
	[[nodiscard]] const auto& getAllow() const { return allow; }
	[[nodiscard]] const auto& getEffect() const { return effect; }
	[[nodiscard]] const auto& getAiWillDo() const { return aiWillDo; }

	friend std::ostream& operator<<(std::ostream& output, const Decision& decision);

  private:
	std::string title;
	std::string picture;
	std::string news;
	std::string potential;
	std::string allow;
	std::string effect;
	std::string aiWillDo;
};
} // namespace V2


#endif // DECISION_H