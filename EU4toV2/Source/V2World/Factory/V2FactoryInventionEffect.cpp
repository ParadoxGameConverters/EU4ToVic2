#include "V2FactoryInventionEffect.h"
#include "V2FactoryInventionDetails.h"
#include "ParserHelpers.h"


mappers::V2FactoryInventionEffect::V2FactoryInventionEffect(std::istream& theStream)
{
	registerKeyword(std::regex("effect"), [this](const std::string& unused, std::istream& theStream)
		{
			V2FactoryInventionDetails effectBlock(theStream);
			factoryName = effectBlock.getFactoryName();
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}
