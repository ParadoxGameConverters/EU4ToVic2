#include "NationMergeParser.h"
#include "ParserHelpers.h"

EU4::NationMergeParser::NationMergeParser()
{
	registerKeyword(std::regex("merge_daimyos"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString daimyosStr(theStream);
			mergeDaimyos = (daimyosStr.getString() == "yes");
		});
	registerKeyword(std::regex("[a-z_]+"), [this](const std::string& unused, std::istream& theStream)
		{
			MergeBlock newBlock(theStream);
			mergeBlocks.push_back(newBlock);
		});
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseFile("merge_nations.txt");
}

