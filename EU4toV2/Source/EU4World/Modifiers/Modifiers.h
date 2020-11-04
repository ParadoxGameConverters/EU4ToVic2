#ifndef MODIFIERS_H
#define MODIFIERS_H
#include "Modifier.h"
#include "Parser.h"
#include <map>

namespace EU4
{
class Modifiers: commonItems::parser
{
  public:
	Modifiers() = default;
	explicit Modifiers(std::istream& theStream);

	void initialize();
	[[nodiscard]] std::optional<Modifier> getModifier(const std::string& modifierName) const;

  private:
	void registerKeys();
	void processFolder(const std::string& folderName);

	std::map<std::string, Modifier> modifiers;
};
} // namespace EU4

#endif // MODIFIERS_H