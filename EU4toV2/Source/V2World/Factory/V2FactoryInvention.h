#ifndef V2_FACTORY_INVENTION_H_
#define V2_FACTORY_INVENTION_H_

#include "newParser.h"
#include <map>

namespace mappers
{

	class V2FactoryInvention : commonItems::parser
	{
	public:
		V2FactoryInvention(std::istream& theStream);
		std::map<std::string, std::string> getFactoryInventionMap() const { return factoryInventionMap; }

	private:
		std::map<std::string, std::string> factoryInventionMap;
	};
}

#endif // V2_FACTORY_INVENTION_H_