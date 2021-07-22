#include "ProvinceRename.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

mappers::ProvinceRename::ProvinceRename(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::ProvinceRename::registerKeys()
{
	registerSetter("province", provinceID);
	registerSetter("name", name);
	registerSetter("culture", culture);
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
