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
	registerKeyword("link", [this](const std::string& unused, std::istream& theStream) 
		{
			const CulturalUnion newUnion(theStream);
			unionMap.insert(newUnion.getUnion());
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

}

std::optional<std::vector<std::string>> mappers::CulturalUnionMapper::getCoresForCulture(const std::string& culture) const
{
	const auto& mapping = unionMap.find(culture);
	if (mapping == unionMap.end()) return std::nullopt;
	return mapping->second;
}
