#ifndef TECHNOLOGIES_H
#define TECHNOLOGIES_H

#include "Parser.h"
#include "TechYearMapper.h"
#include "Technology.h"

namespace mappers
{

class Technologies: commonItems::parser
{
  public:
	Technologies();

	[[nodiscard]] const auto& getCategories() const { return categories; }
	[[nodiscard]] const auto& getTechnologies() const { return technologies; }

  private:
	void registerKeys(const std::string& filename);

	std::map<std::string, std::vector<mappers::Technology>> categories;
	std::vector<mappers::Technology> technologies;
	TechYearMapper techYearMapper;
};


} // namespace mappers


#endif // TECHNOLOGIES_H