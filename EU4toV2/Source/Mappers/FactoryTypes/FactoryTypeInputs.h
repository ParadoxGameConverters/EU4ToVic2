#ifndef FACTORY_TYPE_INPUTS_H
#define FACTORY_TYPE_INPUTS_H

#include "newParser.h"
#include <map>

namespace mappers
{

	class FactoryTypeInputs : commonItems::parser
	{
	public:
		explicit FactoryTypeInputs(std::istream& theStream);
		
		[[nodiscard]] const auto& getProductionInputValues() const { return productionInputValues; }

	private:
		std::map<std::string, double> productionInputValues;
	};
}

#endif // FACTORY_TYPE_INPUTS_H