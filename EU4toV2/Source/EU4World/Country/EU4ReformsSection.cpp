#include "EU4ReformsSection.h"
#include "ParserHelpers.h"

EU4::ReformsSection::ReformsSection(std::istream& theStream)
{
	registerKeyword(std::regex("[a-zA-Z_0-9]+"), [this](const std::string& incomingReform, std::istream& theStream) 
	{
		commonItems::stringList reformList(theStream);
		reforms.insert(reformStr.getString());
	});
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
};

