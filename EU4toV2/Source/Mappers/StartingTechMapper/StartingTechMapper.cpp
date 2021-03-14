#include "StartingTechMapper.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "ParserHelpers.h"

mappers::StartingTechMapper::StartingTechMapper()
{
	LOG(LogLevel::Info) << "Parsing starting techs.";
	registerKeyword("tech", [this](const std::string& techGroup, std::istream& theStream)
		{
			const StartingTech startingTech(theStream);
			startingTechs.emplace_back(startingTech);
		});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	parseFile("configurables/starting_technologies.txt");
	clearRegisteredKeywords();
}

std::vector<std::string> mappers::StartingTechMapper::getTechsForGroupAndScore(const std::string& group, const double score) const
{
	std::vector<std::string> toReturn;
	for (const auto& tech : startingTechs)
	{
		std::string techName = tech.getName();
		if (!tech.getHPM().empty())
			techName = tech.getHPM();
		if (tech.getGroup() == group && score >= tech.getScore())
			toReturn.emplace_back(techName);
	}
	return toReturn;
}
