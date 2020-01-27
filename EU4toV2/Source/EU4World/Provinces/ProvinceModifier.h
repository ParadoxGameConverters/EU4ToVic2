#ifndef PROVINCE_MODIFIER_H
#define PROVINCE_MODIFIER_H

#include "newParser.h"
#include <string>

namespace EU4
{
	class ProvinceModifier: commonItems::parser
	{
	public:
		explicit ProvinceModifier(std::istream& theStream);
		[[nodiscard]] const auto& getModifier() const { return modifier; }

	private:
		std::string modifier;
	};
}

#endif // PROVINCE_MODIFIER_H