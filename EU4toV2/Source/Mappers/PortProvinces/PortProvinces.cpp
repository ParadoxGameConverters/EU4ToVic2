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
	registerKeyword("whitelist", [this](const std::string& provinceID, std::istream& theStream)
		{
			commonItems::intList portList(theStream);
			auto portV = portList.getInts();
			whitelist = std::set<int>(portV.begin(), portV.end());
		});
	registerKeyword("blacklist", [this](const std::string& provinceID, std::istream& theStream)
		{
			commonItems::intList portList(theStream);
			auto portV = portList.getInts();
			blacklist = std::set<int>(portV.begin(), portV.end());
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}
