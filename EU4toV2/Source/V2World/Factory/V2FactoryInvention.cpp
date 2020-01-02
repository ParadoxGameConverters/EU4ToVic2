#include "V2FactoryInvention.h"
#include "V2FactoryInventionEffect.h"
#include "ParserHelpers.h"


mappers::V2FactoryInvention::V2FactoryInvention(std::istream& theStream)
{
	registerKeyword(std::regex("[a-z_]+"), [this](const std::string& invention, std::istream& theStream)
		{
			V2FactoryInventionEffect theEffect(theStream);
			if (!theEffect.getFactoryName().empty())
			{
				factoryInventionMap.insert(std::make_pair(theEffect.getFactoryName(), invention));
			}
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}
