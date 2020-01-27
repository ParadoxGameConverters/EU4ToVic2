#include "ClimateMapper.h"
#include "ParserHelpers.h"
#include "../../Configuration.h"
#include "Log.h"

mappers::ClimateMapper::ClimateMapper()
{
	LOG(LogLevel::Info) << "Loading climates.";
	registerKeys();
	parseFile(theConfiguration.getVic2Path() + "/map/climate.txt");
	clearRegisteredKeywords();
}

mappers::ClimateMapper::ClimateMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::ClimateMapper::registerKeys()
{
	registerRegex("mild_climate|temperate_climate|harsh_climate|inhospitable_climate", [this](const std::string& climate, std::istream& theStream)
		{
			if (!mild_climate && climate == "mild_climate") {
				commonItems::ignoreItem(climate, theStream);
				mild_climate = true;
				return;
			}
			if (!temperate_climate && climate == "temperate_climate") {
				commonItems::ignoreItem(climate, theStream);
				temperate_climate = true;
				return;
			}
			if (!harsh_climate && climate == "harsh_climate") {
				commonItems::ignoreItem(climate, theStream);
				harsh_climate = true;
				return;
			}
			if (!inhospitable_climate && climate == "inhospitable_climate") {
				commonItems::ignoreItem(climate, theStream);
				inhospitable_climate = true;
				return;
			}
			const commonItems::intList provList(theStream);
			climateMap.insert(std::make_pair(climate, provList.getInts()));
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}
