#include "ReligionMapper.h"
#include "ReligionMapping.h"
#include "Log.h"
#include "ParserHelpers.h"

mappers::ReligionMapper::ReligionMapper()
{
	LOG(LogLevel::Info) << "Parsing religion mappings.";
	registerKeys();
	parseFile("configurables/religion_map.txt");
	clearRegisteredKeywords();
}

mappers::ReligionMapper::ReligionMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::ReligionMapper::registerKeys()
{
	registerKeyword("link", [this](const std::string& unused, std::istream& theStream)
		{
			ReligionMapping theMapping(theStream);
			for (auto EU4Religion : theMapping.getEU4Religions())
			{
				EU4ToVic2ReligionMap.insert(make_pair(EU4Religion, theMapping.getVic2Religion()));
			}
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}

std::optional<std::string> mappers::ReligionMapper::getVic2Religion(const std::string& EU4Religion) const
{
	auto mapping = EU4ToVic2ReligionMap.find(EU4Religion);
	if (mapping != EU4ToVic2ReligionMap.end()) return mapping->second;
	return std::nullopt;
}