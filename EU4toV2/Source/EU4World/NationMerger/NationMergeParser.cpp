#include "NationMergeParser.h"
#include "ParserHelpers.h"

EU4::NationMergeParser::NationMergeParser()
{
	registerKeyword("merge_daimyos", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString daimyosStr(theStream);
			mergeDaimyos = (daimyosStr.getString() == "yes");
		});
	registerRegex("[a-z_]+", [this](const std::string& unused, std::istream& theStream)
		{
			MergeBlock newBlock(theStream);
			mergeBlocks.push_back(newBlock);
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseFile("configurables/merge_nations.txt");
	clearRegisteredKeywords();
}

