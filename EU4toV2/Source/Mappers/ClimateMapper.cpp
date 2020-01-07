#include "ClimateMapper.h"
#include "ParserHelpers.h"
#include "../Configuration.h"

mappers::ClimateMapper::ClimateMapper()
{
	registerKeyword(std::regex("mild_climate|temperate_climate|harsh_climate|inhospitable_climate"), [this](const std::string& climate, std::istream& theStream)
	{
			if (!mild_climate && (climate == "mild_climate")) {
				commonItems::ignoreItem(climate, theStream);
				mild_climate = true;
				return;
			}
			if (!temperate_climate && (climate == "temperate_climate")) {
				commonItems::ignoreItem(climate, theStream);
				temperate_climate = true;
				return;
			}
			if (!harsh_climate && (climate == "harsh_climate")) {
				commonItems::ignoreItem(climate, theStream);
				harsh_climate = true;
				return;
			}
			if (!inhospitable_climate && (climate == "inhospitable_climate")) {
				commonItems::ignoreItem(climate, theStream);
				inhospitable_climate = true;
				return;
			}
			commonItems::intList provList(theStream);
			std::vector<int> provStorage;
			for (const auto& provinceID : provList.getInts()) provStorage.push_back(provinceID);
			climateMap.insert(std::make_pair(climate, provStorage));
	});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	std::string filename = theConfiguration.getVic2Path() + "/map/climate.txt";
	parseFile(filename);
}
