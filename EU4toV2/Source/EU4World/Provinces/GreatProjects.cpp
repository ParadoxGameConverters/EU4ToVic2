#include "GreatProjects.h"
#include "ParserHelpers.h"

EU4::GreatProjects::GreatProjects(std::istream& theStream)
{
	registerKeyword(std::regex("[a-z]+_canal"), [this](const std::string& greatProject, std::istream& theStream) {
		commonItems::ignoreItem(greatProject, theStream);
		greatProjects.insert(greatProject);
	});
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}
