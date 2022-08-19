#ifndef CUSTOM_POP_SHAPING_H
#define CUSTOM_POP_SHAPING_H
#include "Configuration.h"
#include "Parser.h"

namespace mappers
{

class CustomPopShaping: commonItems::parser
{

  public:
	CustomPopShaping() = default;
	explicit CustomPopShaping(std::istream& theStream);

	void loadFile(const std::string& fileName);
	void popShapeTypesForRegions(std::istream& theStream);

	[[nodiscard]] std::string getPopShapeType(int provinceID) const;

  private:
	void registerKeys();

	std::map<std::string, std::set<int>> popShapeTypes;
};

} // namespace mappers

#endif // CUSTOM_POP_SHAPING_H
