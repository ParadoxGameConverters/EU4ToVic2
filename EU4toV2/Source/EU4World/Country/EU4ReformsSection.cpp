#include "EU4ReformsSection.h"
#include "ParserHelpers.h"

EU4::reformsSection::reformsSection(std::istream& theStream)
{
	registerKeyword(std::regex("\"[a-z_0-9]+\""), [this](const std::string& incomingReform, std::istream& theStream) 
	{
		std::string governmentReform = incomingReform.substr(1, incomingReform.size() - 2);
		reforms.insert(governmentReform);
	});
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), commonItems::ignoreItem);

	parseStream(theStream);
};

