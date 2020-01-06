#ifndef EU4_MODIFIER_H_
#define EU4_MODIFIER_H_

#include "newParser.h"

namespace EU4
{
	class EU4Modifier: commonItems::parser
	{
		public:
			EU4Modifier() = default;
			EU4Modifier(std::istream& theStream);
			std::string getModifier() const { return modifier; }

		private:
			std::string modifier;
	};
}

#endif // EU4_MODIFIER_H_