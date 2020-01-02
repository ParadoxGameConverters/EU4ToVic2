#include "ColonialTagsMapper.h"
#include "Log.h"
#include "newParser.h"
#include <fstream>


mappers::ColonialTagMapper::ColonialTagMapper()
{
	LOG(LogLevel::Info) << "Parsing colony naming rules.";

	registerKeyword(std::regex("link"), [this](const std::string& unused, std::istream& theStream)
		{
			ColonialTag colonialBlock(theStream);
			colonyList.push_back(colonialBlock.getColonyTag());
		});

	parseFile("colonial_tags.txt");
}
