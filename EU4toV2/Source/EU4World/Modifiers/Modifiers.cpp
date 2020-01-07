#include "Modifiers.h"
#include "ParserHelpers.h"

EU4::Modifiers::Modifiers(std::string filename)
{
	registerKeys();
	parseFile(filename);
}

EU4::Modifiers::Modifiers(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
}

void EU4::Modifiers::addModifiersFile(std::string filename)
{
	registerKeys();
	parseFile(filename);
}

void EU4::Modifiers::registerKeys()
{
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this](const std::string& modifierName, std::istream& theStream) {
		Modifier modifier(theStream);
		modifiers.insert(std::make_pair(modifierName, modifier));
	});
}

std::optional<EU4::Modifier> EU4::Modifiers::getModifier(const std::string& modifierName) const
{
	if (modifiers.count(modifierName) > 0) return std::make_optional(modifiers.at(modifierName));

	return std::nullopt;
}