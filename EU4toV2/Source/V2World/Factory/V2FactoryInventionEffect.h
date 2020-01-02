#ifndef V2_FACTORY_INVENTION_EFFECT_H_
#define V2_FACTORY_INVENTION_EFFECT_H_

#include "newParser.h"


namespace mappers
{
	class V2FactoryInventionEffect : commonItems::parser
	{
	public:
		V2FactoryInventionEffect(std::istream& theStream);
		std::string getFactoryName() const { return factoryName; }

	private:
		std::string factoryName;
	};
}

#endif // V2_FACTORY_INVENTION_EFFECT_H_