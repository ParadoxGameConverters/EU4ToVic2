#include "Mod.h"
#include "ParserHelpers.h"

EU4::Mod::Mod(std::istream& theStream)
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameString(theStream);
		name = nameString.getString();
	});
	registerKeyword("path", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString pathString(theStream);
		path = pathString.getString();
	});
	registerKeyword("archive", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString pathString(theStream);
		path = pathString.getString();
	});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();

	if (!path.empty())
	{
		const auto lastDot = path.find_last_of('.');
		if (lastDot != std::string::npos)
		{
			const auto ending = path.substr(lastDot + 1, path.size());
			compressed = ending == "zip" || ending == "bin";
		}
	}
}