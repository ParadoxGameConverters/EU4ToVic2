#include "LeaderTraitTypes.h"
#include "ParserHelpers.h"
#include "Log.h"

mappers::LeaderTraitTypes::LeaderTraitTypes(std::istream& theStream)
{
	registerRegex("[a-z_]+", [this](const std::string& traitName, std::istream& theStream)
		{
			const LeaderTrait traitBlock(theStream);
			traits.emplace_back(std::make_pair(traitName, traitBlock));
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
