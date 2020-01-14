#include "NavalBaseMapper.h"
#include "BuildingPosition.h"
#include "ParserHelpers.h"
#include "../../Configuration.h"
#include "Log.h"

mappers::NavalBaseMapper::NavalBaseMapper()
{
	LOG(LogLevel::Info) << "Finding coastal provinces.";

	registerRegex("[0-9]+", [this](const std::string& provinceID, std::istream& theStream)
		{
			mappers::BuildingPosition possibleNaval(theStream);
			if (possibleNaval.getFound()) navalProvinces.insert(stoi(provinceID));
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseFile(theConfiguration.getVic2Path() + "/map/positions.txt");
	clearRegisteredKeywords();
}
