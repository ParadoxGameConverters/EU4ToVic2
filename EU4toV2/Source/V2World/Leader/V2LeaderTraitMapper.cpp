#include "V2LeaderTraitMapper.h"
#include "V2LeaderTraitTypes.h"
#include "ParserHelpers.h"


mappers::V2LeaderTraitMapper::V2LeaderTraitMapper()
{
	registerKeyword(std::regex("personality"), [this](const std::string& traitName, std::istream& theStream)
		{
			V2LeaderTraitTypes traitBlock(theStream);
			personalities = traitBlock.getTraits();
		});
	registerKeyword(std::regex("background"), [this](const std::string& traitName, std::istream& theStream)
		{
			V2LeaderTraitTypes traitBlock(theStream);
			backgrounds = traitBlock.getTraits();
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseFile("leader_traits.txt");
}

std::optional<std::string> mappers::V2LeaderTraitMapper::getBackground(int fire, int shock, int manuever, int siege) const
{
	for (std::vector<std::pair<std::string, V2LeaderTrait>>::const_iterator traitItr = backgrounds.begin(); traitItr != backgrounds.end(); ++traitItr)
	{
		if (traitItr->second.matches(fire, shock, manuever, siege)) return traitItr->first;
	}
	return {};
}

std::optional<std::string> mappers::V2LeaderTraitMapper::getPersonality(int fire, int shock, int manuever, int siege) const
{
	for (std::vector<std::pair<std::string, V2LeaderTrait>>::const_iterator traitItr = personalities.begin(); traitItr != personalities.end(); ++traitItr)
	{
		if (traitItr->second.matches(fire, shock, manuever, siege)) return traitItr->first;
	}
	return {};
}
