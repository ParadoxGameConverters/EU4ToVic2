#include "Issues.h"
#include "CommonRegexes.h"
#include "Configuration.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include <Log.h>

mappers::Issues::Issues()
{
	Log(LogLevel::Info) << "Parsing issues";
	registerKeys();
	parseFile(theConfiguration.getVic2Path() + "/common/issues.txt");
	clearRegisteredKeywords();
}

void mappers::Issues::registerKeys()
{
	registerRegex("[a-z_]+", [this](const std::string& categoryName, std::istream& theStream) {
		Log(LogLevel::Info) << " -> " << categoryName;
		commonItems::stringsOfItemNames issuesStr(theStream);
		const auto& issues = issuesStr.getStrings();
		categories.insert(std::make_pair(categoryName, issues));
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}