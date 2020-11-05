#ifndef TECHNOLOGIES_H
#define TECHNOLOGIES_H

#include "Parser.h"
#include "TechYearMapper.h"
#include "StartingTechMapper.h"
#include "Technology.h"

namespace mappers
{

class Technologies: commonItems::parser
{
  public:
	Technologies();

	void updateTechs();
	void setTechLevels();
	void makeIdNameMap();

	[[nodiscard]] const auto& getCategories() const { return categories; }
	[[nodiscard]] const auto& getTechsInCategory() const { return techsInCategory; }
	[[nodiscard]] const auto& getStartingTechMapper() const { return startingTechMapper; }
	[[nodiscard]] const auto& getIdNameMap() const { return idNameMap; }

  private:
	void registerKeys(const std::string& filename);

	std::map<std::string, std::vector<mappers::Technology>> categories;
	std::vector<mappers::Technology> techsInCategory;
	TechYearMapper techYearMapper;
	StartingTechMapper startingTechMapper;
	std::map<std::string, std::string> idNameMap;
};


} // namespace mappers


#endif // TECHNOLOGIES_H