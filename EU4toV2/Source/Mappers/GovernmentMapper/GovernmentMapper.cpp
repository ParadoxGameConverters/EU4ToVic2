#include "GovernmentMapper.h"
#include "Log.h"
#include "ParserHelpers.h"

mappers::GovernmentMapper::GovernmentMapper()
{
	LOG(LogLevel::Info) << "Parsing governments mappings.";
	registerKeys();
	parseFile("configurables/government_mapping.txt");
	clearRegisteredKeywords();
}

mappers::GovernmentMapper::GovernmentMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::GovernmentMapper::registerKeys()
{
	registerRegex("[a-z_]+", [this](const std::string& sourceGov, std::istream& theStream)
		{
			commonItems::stringList govList(theStream);
			for (auto destGov : govList.getStrings()) {
				governmentMap.insert(make_pair(destGov, sourceGov));
			}
		});
	registerRegex("[a-z0-9\\_]+", commonItems::ignoreItem);
}

std::string mappers::GovernmentMapper::matchGovernment(const std::string& sourceGovernment) const
{
	auto mapping = governmentMap.find(sourceGovernment);
	if (mapping != governmentMap.end())
	{
		return mapping->second;
	}
	else
	{
		LOG(LogLevel::Warning) << "No government mapping defined for " << sourceGovernment;
		return "";
	}
}
