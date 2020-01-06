#include "EU4ReformStackSection.h"
#include "EU4ReformsSection.h"
#include "ParserHelpers.h"

EU4::reformStackSection::reformStackSection(std::istream& theStream)
{
	registerKeyword(std::regex("reforms"), [this](const std::string& unused, std::istream& theStream)
	{
		reforms = reformStackSection::readStack(theStream);
	});
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), commonItems::ignoreItem);

	parseStream(theStream);
}

