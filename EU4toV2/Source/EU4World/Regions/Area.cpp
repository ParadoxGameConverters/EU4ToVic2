#include "Area.h"

EU4::Area::Area(std::istream& theStream)
{
	registerKeyword("color", [this](const std::string & colorToken, std::istream & areaFile)
	{
		commonItems::Color newColor(areaFile);
	});
	registerKeyword(std::regex("[0-9]+"), [this](const std::string & number, std::istream & theStream) {
		provinces.insert(std::stoi(number));
	});

	parseStream(theStream);
	clearRegisteredKeywords();
}