#ifndef MODIFIERS_H
#define MODIFIERS_H

#include "Modifier.h"
#include "newParser.h"
#include <map>

namespace EU4
{
	class Modifiers: commonItems::parser
	{
	public:
		Modifiers();
		Modifiers(std::istream& theStream);
		void registerKeys();
		void initialize();
		void processFolder(const std::string& folderName);
		std::optional<Modifier> getModifier(const std::string& modifierName) const;

	private:
		std::map<std::string, Modifier> modifiers;
	};
}

#endif // MODIFIERS_H