#include "ProvinceRenamings.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "ProvinceRename.h"

mappers::ProvinceRenamings::ProvinceRenamings()
{
	registerKeys();
	parseFile("configurables/province_renamings.txt");
	clearRegisteredKeywords();
}

mappers::ProvinceRenamings::ProvinceRenamings(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::ProvinceRenamings::registerKeys()
{
	registerKeyword("link", [this](std::istream& theStream) {
		const ProvinceRename mapping(theStream);
		if (renames.contains(mapping.getProvinceID()))
		{
			if (renames.at(mapping.getProvinceID()).contains(mapping.getCulture()))
				renames.at(mapping.getProvinceID()).at(mapping.getCulture()) = mapping.getName();
			else
				renames.at(mapping.getProvinceID()).emplace(mapping.getCulture(), mapping.getName());
		}
		else
		{
			renames.emplace(mapping.getProvinceID(), std::map<std::string, std::string>{});
			renames.at(mapping.getProvinceID()).emplace(mapping.getCulture(), mapping.getName());
		}
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

std::optional<std::string> mappers::ProvinceRenamings::getRename(int provinceID, const std::string& culture) const
{
	if (!renames.contains(provinceID))
		return std::nullopt;
	if (!renames.at(provinceID).contains(culture))
		return std::nullopt;
	return renames.at(provinceID).at(culture);
}
