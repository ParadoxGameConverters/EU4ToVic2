#ifndef MODIFIER_H
#define MODIFIER_H

#include "newParser.h"
#include <map>

namespace EU4
{
	class Modifier: commonItems::parser
	{
	public:
		Modifier() = default;
		Modifier(std::istream& theStream);
		double getEffectAmount(const std::string& modifier) const;

		const std::map<std::string, double>& getAllEffects() const { return effects; }

	private:
		std::map<std::string, double> effects;
	};

}



#endif // MODIFIER_H