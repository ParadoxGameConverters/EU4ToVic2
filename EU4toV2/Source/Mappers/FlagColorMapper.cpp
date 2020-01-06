#include "FlagColorMapper.h"
#include "ParserHelpers.h"

mappers::FlagColorMapper::FlagColorMapper(std::istream& theStream)
{
	registerKeyword(std::regex("flag_color"), [this](const std::string& sourceGov, std::istream& theStream)
	{
		commonItems::Color theColor(theStream);
		flagColorMapping.push_back(theColor);
	});
	registerKeyword(std::regex("[a-z0-9\\_]+"), commonItems::ignoreItem);

	parseStream(theStream);
}

std::optional<commonItems::Color> mappers::FlagColorMapper::getFlagColorByIndex(int index) const 
{ 
	if ((flagColorMapping.empty()) || (static_cast<long>(index) >= static_cast<long>(flagColorMapping.size()))) return std::nullopt;
	return flagColorMapping[index]; 
}
