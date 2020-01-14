#include "CulturalUnionMapper.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "CulturalUnion.h"

mappers::CulturalUnionMapper::CulturalUnionMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::CulturalUnionMapper::loadFile(const std::string& filename)
{
	registerKeys();
	parseFile(filename);
	clearRegisteredKeywords();
}

void mappers::CulturalUnionMapper::registerKeys()
{
	registerKeyword(std::regex("link"), [this](const std::string& unused, std::istream& theStream) 
		{
			CulturalUnion newUnion(theStream);
			unionMap.insert(newUnion.getUnion());
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

}

std::vector<std::string> mappers::CulturalUnionMapper::getCoresForCulture(const std::string& culture) const
{
	auto mapping = unionMap.find(culture);
	if (mapping == unionMap.end())
	{
		std::vector<std::string> empty;
		return empty;
	}
	else
	{
		return mapping->second;
	}
}
