#include "PortProvinces.h"
#include "Log.h"
#include "ParserHelpers.h"

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
	registerKeyword("whitelist", [this](const std::string& unused, std::istream& theStream) {
		const auto portVector = commonItems::intList(theStream).getInts();
		whitelist.insert(portVector.begin(), portVector.end());
	});
	registerKeyword("blacklist", [this](const std::string& unused, std::istream& theStream) {
		auto portVector = commonItems::intList(theStream).getInts();
		blacklist.insert(portVector.begin(), portVector.end());
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
