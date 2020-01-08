#include "Continents.h"
#include "../Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

mappers::Continents::Continents()
{
	registerKeyword(std::regex("\\w+"), [this](const std::string& continentName, std::istream& theStream)
		{
			commonItems::intList newContinentList(theStream);
			for (const auto& provinceID: newContinentList.getInts()) continentMap.insert(std::make_pair(provinceID, continentName));
		});

	LOG(LogLevel::Info) << "Finding Continents";
	for (auto mod: theConfiguration.getEU4Mods())
	{
		std::string continentFile = mod + "/map/continent.txt";
		if (Utils::DoesFileExist(continentFile)) parseFile(continentFile);
	}
	if (continentMap.empty()) parseFile(theConfiguration.getEU4Path() + "/map/continent.txt");
	if (continentMap.empty()) LOG(LogLevel::Warning) << "No continent mappings found - may lead to problems later";
}

std::optional<std::string> mappers::Continents::getEU4Continent(int EU4Province)
{
	auto mapping = continentMap.find(EU4Province);
	if (mapping != continentMap.end()) return mapping->second;
	return {};
}