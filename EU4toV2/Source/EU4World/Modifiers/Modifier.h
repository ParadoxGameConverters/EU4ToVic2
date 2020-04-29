#ifndef MODIFIER_H
#define MODIFIER_H

#include "Parser.h"
#include <map>

namespace EU4
{
	class Modifier: commonItems::parser
	{
	public:
		Modifier() = default;
		explicit Modifier(std::istream& theStream);
		
		[[nodiscard]] double getEffectAmount(const std::string& modifier) const;

		[[nodiscard]] const auto& getAllEffects() const { return effects; }

	private:
		std::map<std::string, double> effects;
	};

}

#endif // MODIFIER_H