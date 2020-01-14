#include "ColonialFlag.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "OSCompatibilityLayer.h"

mappers::ColonialFlag::ColonialFlag(std::istream& theStream, const std::string& region)
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString nameStr(theStream);
			name = Utils::convertUTF8To8859_15(nameStr.getString());
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);
		});
	registerKeyword("unique", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::ignoreItem(unused, theStream);
			unique = true;
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}

