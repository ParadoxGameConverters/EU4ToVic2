#include "Area.h"
#include "ParserHelpers.h"

EU4::Area::Area(std::istream& theStream)
{
	registerKeyword("color", commonItems::ignoreItem);
	registerRegex("[0-9]+", [this](const std::string& number, std::istream& theStream) {
		// This is a peculiar file format where we pull free-floating numbers from thin air
		provinces.insert(std::stoi(number));
	});

	parseStream(theStream);
	clearRegisteredKeywords();
}
