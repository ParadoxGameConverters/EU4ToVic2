#include "V2LeaderTraitTypes.h"
#include "ParserHelpers.h"
#include "Log.h"

mappers::V2LeaderTraitTypes::V2LeaderTraitTypes(std::istream& theStream)
{
	registerKeyword(std::regex("[a-z_]+"), [this](const std::string& traitName, std::istream& theStream)
		{
			V2LeaderTrait traitBlock(theStream);
			traits.push_back(std::make_pair(traitName, traitBlock));
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}
