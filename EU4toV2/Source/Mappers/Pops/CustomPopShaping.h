#ifndef CUSTOM_POP_SHAPING_H
#define CUSTOM_POP_SHAPING_H
#include "Parser.h"
#include "Configuration.h"

namespace mappers
{

class CustomPopShaping: commonItems::parser
{

  public:
	CustomPopShaping() = default;
	explicit CustomPopShaping(std::istream& theStream);

	void loadFile(const std::string& fileName);
	void popShapeTypesForRegions(std::istream& theStream);

	[[nodiscard]] auto getPopShapeTypes() const { return popShapeTypes; }

  private:
	void registerKeys();

	bool usingHPM = false;
	std::string pop_shaping;

	std::map<std::string, std::set<int>> popShapeTypes;
};
} // namespace mappers

#endif // CUSTOM_POP_SHAPING_H
