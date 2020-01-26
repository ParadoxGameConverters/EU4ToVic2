#include "PortProvinces.h"
#include "ParserHelpers.h"
#include "Log.h"

mappers::PortProvinces::PortProvinces()
{
	Log(LogLevel::Info) << "Parsing black and white ports.";
	registerKeys();
	parseFile("configurables/port_provinces.txt");
	clearRegisteredKeywords();
}

mappers::PortProvinces::PortProvinces(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::PortProvinces::registerKeys()
{
	registerKeyword("whitelist", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::intList portList(theStream);
			auto portVector = portList.getInts();
			whitelist = std::set<int>(portVector.begin(), portVector.end());
		});
	registerKeyword("blacklist", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::intList portList(theStream);
			auto portVector = portList.getInts();
			blacklist = std::set<int>(portVector.begin(), portVector.end());
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}
