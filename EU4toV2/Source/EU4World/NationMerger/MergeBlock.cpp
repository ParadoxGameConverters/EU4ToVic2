#include "MergeBlock.h"
#include "ParserHelpers.h"

EU4::MergeBlock::MergeBlock(std::istream& theStream)
{
	registerKeyword(std::regex("master"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString masterStr(theStream);
			master = masterStr.getString();
		});
	registerKeyword(std::regex("slave"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString slaveStr(theStream);
			slaves.insert(slaveStr.getString());
		});
	registerKeyword(std::regex("merge"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString mergeStr(theStream);
			merge = (mergeStr.getString() == "yes");
		});
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}

