#ifndef V2_FACTORY_TYPE_INPUTS_H_
#define V2_FACTORY_TYPE_INPUTS_H_

#include "newParser.h"
#include <map>


namespace mappers
{

	class V2FactoryTypeInputs : commonItems::parser
	{
	public:
		V2FactoryTypeInputs(std::istream& theStream);
		std::map<std::string, double> getProductionInputValues() const { return productionInputValues; }

	private:
		std::map<std::string, double> productionInputValues;
	};
}

#endif // V2_FACTORY_TYPE_INPUTS_H_