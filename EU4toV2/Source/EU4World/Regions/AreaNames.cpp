#include "AreaNames.h"

EU4::AreaNames::AreaNames(std::istream& theStream)
{
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this](const std::string & areaName, std::istream & theStream) {
		names.insert(areaName);
	});

	parseStream(theStream);
}