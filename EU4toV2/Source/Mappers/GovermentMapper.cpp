#include "GovernmentMapper.h"
#include "Log.h"
#include "ParserHelpers.h"


mappers::GovernmentMapper::GovernmentMapper(std::istream& theStream)
{
	registerKeyword(std::regex("[a-z_]+"), [this](const std::string& sourceGov, std::istream& theStream)
	{
		commonItems::stringList govList(theStream);
		for (auto destGov : govList.getStrings()) {
			governmentMap.insert(make_pair(destGov, sourceGov));
		}
	});
	registerKeyword(std::regex("[a-z0-9\\_]+"), commonItems::ignoreItem);

	parseStream(theStream);
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
