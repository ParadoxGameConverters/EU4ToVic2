#include "ColonialTagsMapper.h"
#include "newParser.h"
#include <fstream>
#include "ParserHelpers.h"

mappers::ColonialTagMapper::ColonialTagMapper()
{
	registerKeys();
	parseFile("configurables/colonial_tags.txt");
	clearRegisteredKeywords();
}

mappers::ColonialTagMapper::ColonialTagMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::ColonialTagMapper::registerKeys()
{
	registerKeyword("link", [this](const std::string& unused, std::istream& theStream)
		{
			ColonialTag colonialBlock(theStream);
			colonyList.push_back(colonialBlock.getColonyTag());
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}