#include "CulturalUnionMapper.h"
#include "CulturalUnion.h"
#include "ParserHelpers.h"

mappers::CulturalUnionMapper::CulturalUnionMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

mappers::CulturalUnionMapper::CulturalUnionMapper(const std::string& filename)
{
	registerKeys();
	parseFile(filename);
	clearRegisteredKeywords();
}

void mappers::CulturalUnionMapper::registerKeys()
{
	registerKeyword("link", [this](const std::string& unused, std::istream& theStream) {
		const CulturalUnion newUnion(theStream);
		unionMap.insert(std::make_pair(newUnion.getCulture(), newUnion.getTags()));
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

std::vector<std::string> mappers::CulturalUnionMapper::getCoresForCulture(const std::string& culture) const
{

	if (const auto& mapping = unionMap.find(culture); mapping != unionMap.end())
		return mapping->second;
	else
		return std::vector<std::string>();
}
