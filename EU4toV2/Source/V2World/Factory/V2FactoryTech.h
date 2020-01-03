#ifndef V2_FACTORY_TECH_H_
#define V2_FACTORY_TECH_H_

#include "newParser.h"
#include <map>


namespace mappers
{
	class V2FactoryTech : commonItems::parser
	{
	public:
		V2FactoryTech(std::istream& theStream);
		std::map<std::string, std::string> getFactoryTechMap() const { return factoryTechMap; }

	private:
		std::map<std::string, std::string> factoryTechMap;
	};
}

#endif // V2_FACTORY_TECH_H_