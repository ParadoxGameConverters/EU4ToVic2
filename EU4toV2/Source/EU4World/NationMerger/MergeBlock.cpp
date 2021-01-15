#include "MergeBlock.h"
#include "ParserHelpers.h"
#include "CommonRegexes.h"

EU4::MergeBlock::MergeBlock(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU4::MergeBlock::registerKeys()
{
	registerKeyword("master", [this](const std::string& unused, std::istream& theStream) {
		master = commonItems::singleString(theStream).getString();
	});
	registerKeyword("slave", [this](const std::string& unused, std::istream& theStream) {
		slaves.insert(commonItems::singleString(theStream).getString());
	});
	registerKeyword("merge", [this](const std::string& unused, std::istream& theStream) {
		merge = commonItems::singleString(theStream).getString() == "yes";
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
