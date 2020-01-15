#ifndef FACTORY_INVENTION_EFFECT_H
#define FACTORY_INVENTION_EFFECT_H

#include "newParser.h"

namespace mappers
{
	class FactoryInventionEffect : commonItems::parser
	{
	public:
		FactoryInventionEffect(std::istream& theStream);
		std::string getFactoryName() const { return factoryName; }

	private:
		std::string factoryName;
	};
}

#endif // FACTORY_INVENTION_EFFECT_H