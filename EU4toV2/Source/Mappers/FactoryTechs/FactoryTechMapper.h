#ifndef FACTORY_TECH_MAPPER_H
#define FACTORY_TECH_MAPPER_H
#include "Parser.h"

namespace mappers
{
class FactoryTechMapper: commonItems::parser
{
  public:
	FactoryTechMapper();
	explicit FactoryTechMapper(std::istream& theStream);

	[[nodiscard]] const auto& getFactoryTechMap() const { return factoryTechMap; }

  private:
	void registerKeys();

	std::map<std::string, std::string> factoryTechMap;
};
} // namespace mappers

#endif // FACTORY_TECH_MAPPER_H