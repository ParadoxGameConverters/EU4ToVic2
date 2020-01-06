#include "EU4GovernmentSection.h"
#include "ParserHelpers.h"

EU4::governmentSection::governmentSection(std::istream& theStream)
{
	registerKeyword(std::regex("government"), [this](const std::string& unused, std::istream& theStream)
		{
			government = governmentSection::readGovernment(theStream);
		});
	registerKeyword(std::regex("reform_stack"), [this](const std::string& unused, std::istream& theStream)
		{
			reformStack = governmentSection::readGovernmentReforms(theStream);
		});
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), commonItems::ignoreItem);

	parseStream(theStream);
}