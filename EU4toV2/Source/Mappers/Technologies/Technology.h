#ifndef TECHNOLOGY_H
#define TECHNOLOGY_H

#include "Parser.h"
#include "TechYearMapper.h"

namespace mappers
{

class Technology: commonItems::parser
{
  public:
	Technology(std::istream& theStream);

	void setTechName(const std::string& techName) { name = techName; }
	void setYear(const std::string& techYear) { year = stoi(techYear); }
	void setLevel(int techLevel) { level = techLevel; }

	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getArea() const { return area; }
	[[nodiscard]] const auto& getLevel() const { return level; }
	[[nodiscard]] const auto& getYear() const { return year; }
	[[nodiscard]] const auto& getCost() const { return cost; }
	[[nodiscard]] const auto& getUncivMilitary() const { return uncivMilitary; }
	[[nodiscard]] const auto& getEffects() const { return effects; }
	[[nodiscard]] const auto& getAiChance() const { return aiChance; }

	friend std::ostream& operator<<(std::ostream& output, const Technology& technology);

  private:
	void registerKeys();

	std::string name;
	std::string area;
	int level;
	int year;
	int cost;
	std::string uncivMilitary;
	std::vector<std::string> effects;
	std::string aiChance;
};


} // namespace mappers


#endif // TECHNOLOGY_H