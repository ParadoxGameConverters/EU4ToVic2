#include "FlagColorMapper.h"
#include "Log.h"
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
