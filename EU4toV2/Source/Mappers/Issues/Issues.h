#ifndef ISSUES_H
#define ISSUES_H

#include "Parser.h"

namespace mappers
{
class Issue;

class Issues: commonItems::parser
{
  public:
	Issues();

	void registerKeys();
	std::string determineSrcFile();

	[[nodiscard]] const auto& getCategories() const { return categories; }

  private:
	std::map<std::string, std::vector<std::string>> categories;
};
} // namespace mappers


#endif // ISSUES_H