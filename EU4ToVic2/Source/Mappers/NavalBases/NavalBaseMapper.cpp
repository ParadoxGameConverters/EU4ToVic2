#include "NavalBaseMapper.h"
#include "BuildingPosition.h"
#include "CommonRegexes.h"
#include "Configuration.h"
#include "Log.h"
#include "ParserHelpers.h"

mappers::NavalBaseMapper::NavalBaseMapper()
{
	Log(LogLevel::Info) << "Finding coastal provinces.";
	registerKeys();
	parseFile(theConfiguration.getVic2Path() + "/map/positions.txt");
	clearRegisteredKeywords();
}

mappers::NavalBaseMapper::NavalBaseMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::NavalBaseMapper::registerKeys()
{
	registerRegex(R"(\d+)", [this](const std::string& provinceID, std::istream& theStream) {
		const BuildingPosition possibleNaval(theStream);
		if (possibleNaval.isFound())
			navalProvinces.insert(stoi(provinceID));
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
