#include "Mod.h"
#include "CommonFunctions.h"
#include "ParserHelpers.h"

EU4::Mod::Mod(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();

	if (!path.empty())
	{
		const auto ending = getExtension(path);
		compressed = ending == "zip" || ending == "bin";
	}
}

void EU4::Mod::registerKeys()
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		name = commonItems::singleString(theStream).getString();
	});
	registerRegex("path|archive", [this](const std::string& unused, std::istream& theStream) {
		path = commonItems::singleString(theStream).getString();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
