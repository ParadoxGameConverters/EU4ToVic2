#include "V2FactoryInvention.h"
#include "V2FactoryInventionEffect.h"
#include "ParserHelpers.h"


mappers::V2FactoryInvention::V2FactoryInvention(std::istream& theStream)
{
	// we need èüéö for jean_jaurès, johann_heinrich_von_thünen, léon_walras, eugen_von_böhm_bawerk :/
	registerKeyword(std::regex("[èüéöa-z_]+"), [this](const std::string& invention, std::istream& theStream)
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
