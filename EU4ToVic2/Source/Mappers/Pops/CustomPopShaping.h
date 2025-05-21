#ifndef CUSTOM_POP_SHAPING_H
#define CUSTOM_POP_SHAPING_H
#include "Configuration.h"
#include "Parser.h"

namespace mappers
{

enum class ShapingType
{
	vanilla_type,
	dev_push_type,
	absolute_type
};

class CustomPopShaping: commonItems::parser
{

  public:
	CustomPopShaping() = default;
	explicit CustomPopShaping(std::istream& theStream);

	void loadFile(const std::filesystem::path& fileName);
	void popShapeTypesForRegions(std::istream& theStream);

	[[nodiscard]] ShapingType getPopShapeType(int provinceID) const;

  private:
	void registerKeys();

	std::map<int, ShapingType> popShapeTypes;
};

} // namespace mappers

#endif // CUSTOM_POP_SHAPING_H
