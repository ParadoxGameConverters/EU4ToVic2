#include "LeaderTraitMapper.h"
#include "LeaderTraitTypes.h"
#include "ParserHelpers.h"
#include "Log.h"

mappers::LeaderTraitMapper::LeaderTraitMapper()
{
	LOG(LogLevel::Info) << "Loading leader traits.";
	registerKeys();
	parseFile("configurables/leader_traits.txt");
	clearRegisteredKeywords();
}

mappers::LeaderTraitMapper::LeaderTraitMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::LeaderTraitMapper::registerKeys()
{
	registerKeyword("personality", [this](const std::string& traitName, std::istream& theStream)
		{
			LeaderTraitTypes traitBlock(theStream);
			personalities = traitBlock.getTraits();
		});
	registerKeyword("background", [this](const std::string& traitName, std::istream& theStream)
		{
			LeaderTraitTypes traitBlock(theStream);
			backgrounds = traitBlock.getTraits();
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}

std::optional<std::string> mappers::LeaderTraitMapper::getBackground(const int& fire, const int& shock, const int& maneuver, const int& siege) const
{
	for (auto traitItr: backgrounds)
	{
		if (traitItr.second.matches(fire, shock, maneuver, siege)) return traitItr.first;
	}
	return std::nullopt;
}

std::optional<std::string> mappers::LeaderTraitMapper::getPersonality(const int& fire, const int& shock, const int& maneuver, const int& siege) const
{
	for (auto traitItr: personalities)
	{
		if (traitItr.second.matches(fire, shock, maneuver, siege)) return traitItr.first;
	}
	return std::nullopt;
}
