#include "FactoryInventionEffect.h"
#include "FactoryInventionDetails.h"
#include "ParserHelpers.h"


mappers::FactoryInventionEffect::FactoryInventionEffect(std::istream& theStream)
{
	registerKeyword("effect", [this](const std::string& unused, std::istream& theStream)
		{
			FactoryInventionDetails effectBlock(theStream);
			factoryName = effectBlock.getFactoryName();
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
