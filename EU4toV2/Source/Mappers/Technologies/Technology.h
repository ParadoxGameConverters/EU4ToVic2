#ifndef TECHNOLOGY_H
#define TECHNOLOGY_H

#include "Parser.h"
#include "TechYearMapper.h"

namespace mappers
{

class Technology: commonItems::parser
{
  public:
	Technology(std::istream& theStream, const TechYearMapper& techYearMap);

	void setTechName(const std::string& techName) { name = techName; }
	void updateYear(const std::map<std::string, std::string>& theMap);

	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getArea() const { return area; }
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
	int year;
	int cost;
	std::string uncivMilitary;
	std::vector<std::string> effects;
	std::string aiChance;
};


} // namespace mappers


#endif // TECHNOLOGY_H