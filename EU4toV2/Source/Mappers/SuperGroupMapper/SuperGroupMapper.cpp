#include "SuperGroupMapper.h"
#include "ParserHelpers.h"
#include "Log.h"

mappers::SuperGroupMapper::SuperGroupMapper()
{
	LOG(LogLevel::Info) << "Parsing starting supergroups.";

	registerRegex("[a-z_]+", [this](const std::string& superGroupName, std::istream& theStream)
		{
			const commonItems::stringList superRegionsList(theStream);
			const auto& srlist = superRegionsList.getStrings();
			superGroups[superGroupName].insert(srlist.begin(), srlist.end());
		});
	registerRegex("[a-zA-Z0-9\\_]+", commonItems::ignoreItem);

	parseFile("configurables/world_supergroups.txt");
	clearRegisteredKeywords();
}

std::optional<std::string> mappers::SuperGroupMapper::getGroupForSuperRegion(const std::string& superRegionName) const
{
	for (const auto& superGroup: superGroups) if (superGroup.second.count(superRegionName)) return superGroup.first;
	return std::nullopt;
}

