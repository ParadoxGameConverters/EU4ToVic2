#include "ColonialFlagsMapper.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

mappers::ColonialFlagsMapper::ColonialFlagsMapper()
{
	registerKeys();
	parseFile("colonial_flags.txt");
	clearRegisteredKeywords();
}

mappers::ColonialFlagsMapper::ColonialFlagsMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::ColonialFlagsMapper::registerKeys()
{
	registerRegex("[\\w_]+", [this](const std::string& region, std::istream& theStream)
		{
			ColonialFlagRegion newRegion(theStream, region);
		});
}

std::optional<mappers::ColonialFlag> mappers::ColonialFlagsMapper::getFlag(const std::string& name) const
{
	auto possibleFlag = colonialFlags.find(name);
	if (possibleFlag != colonialFlags.end())
	{
		return possibleFlag->second;
	}
	else
	{
		for (auto flag: colonialFlags)
		{
			if (name.find(flag.first) != std::string::npos)
			{
				return flag.second;
			}
		}
		return std::nullopt;
	}
}

std::vector<std::string> mappers::ColonialFlagsMapper::getNames() const
{
	std::vector<std::string> names;
	for (auto flag: colonialFlags)
	{
		if (!flag.second.isUnique())
		{
			names.push_back(flag.first);
		}
	}
	return names;
}

void mappers::ColonialFlagsMapper::removeFlag(const std::string& name)
{
	colonialFlags.erase(name);
}