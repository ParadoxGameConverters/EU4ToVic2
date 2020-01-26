#ifndef EU4_MODIFIER_H
#define EU4_MODIFIER_H

#include "newParser.h"

namespace EU4
{
	class EU4Modifier: commonItems::parser
	{
	public:
		EU4Modifier() = default;
		explicit EU4Modifier(std::istream& theStream);
		[[nodiscard]] const auto& getModifier() const { return modifier; }

	private:
		std::string modifier;
	};
}

#endif // EU4_MODIFIER_H