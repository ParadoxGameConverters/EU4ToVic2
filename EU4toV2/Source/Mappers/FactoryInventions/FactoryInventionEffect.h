#ifndef FACTORY_INVENTION_EFFECT_H
#define FACTORY_INVENTION_EFFECT_H

#include "newParser.h"

namespace mappers
{
	class FactoryInventionEffect : commonItems::parser
	{
	public:
		explicit FactoryInventionEffect(std::istream& theStream);
		
		[[nodiscard]] const auto& getFactoryName() const { return factoryName; }

	private:
		std::string factoryName;
	};
}

#endif // FACTORY_INVENTION_EFFECT_H