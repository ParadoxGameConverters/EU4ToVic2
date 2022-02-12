#include "CultureGroupsMapper.h"
#include "CommonRegexes.h"
#include "CultureGroupsMapping.h"
#include "Log.h"
#include "ParserHelpers.h"

mappers::CultureGroupsMapper::CultureGroupsMapper()
{
	Log(LogLevel::Info) << "-> Loading culture group equivalences.";
	registerKeys();
	parseFile("configurables/culture_group_map.txt");
	clearRegisteredKeywords();
	Log(LogLevel::Info) << "-> " << eu4ToV2CultureGroups.size() << " links loaded.";
}

mappers::CultureGroupsMapper::CultureGroupsMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::CultureGroupsMapper::registerKeys()
{
	registerKeyword("link", [this](std::istream& theStream) {
		const CultureGroupsMapping mapping(theStream);
		if (!mapping.getV2().empty() && !mapping.getEU4s().empty())
			for (const auto& eu4Group: mapping.getEU4s())
				eu4ToV2CultureGroups.emplace(eu4Group, mapping.getV2());
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

std::optional<std::string> mappers::CultureGroupsMapper::getV2CultureGroup(const std::string& eu4CultureGroup) const
{
	if (eu4ToV2CultureGroups.contains(eu4CultureGroup))
		return eu4ToV2CultureGroups.at(eu4CultureGroup);
	return std::nullopt;
}
