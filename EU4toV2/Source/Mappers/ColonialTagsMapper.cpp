#include "ColonialTagsMapper.h"
#include "Log.h"
#include "newParser.h"
#include <fstream>
#include "ParserHelpers.h"


mappers::ColonialTagMapper::ColonialTagMapper()
{
	LOG(LogLevel::Info) << "Parsing colony naming rules.";

	registerKeyword(std::regex("link"), [this](const std::string& unused, std::istream& theStream)
		{
			ColonialTag colonialBlock(theStream);
			colonyList.push_back(colonialBlock.getColonyTag());
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseFile("colonial_tags.txt");
}
