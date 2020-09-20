#include "Continents.h"
#include "../../Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

mappers::Continents::Continents()
{
	registerKeys();
	LOG(LogLevel::Info) << "Finding Continents";
	for (const auto& mod: theConfiguration.getEU4Mods())
	{
		const auto& continentFile = mod + "/map/continent.txt";
		if (commonItems::DoesFileExist(continentFile)) parseFile(continentFile);
	}
	if (continentMap.empty()) parseFile(theConfiguration.getEU4Path() + "/map/continent.txt");
	if (continentMap.empty()) LOG(LogLevel::Warning) << "No continent mappings found - may lead to problems later";
	clearRegisteredKeywords();
}

mappers::Continents::Continents(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::Continents::registerKeys()
{
	registerRegex("\\w+", [this](const std::string& continentName, std::istream& theStream)
		{
			const commonItems::intList newContinentList(theStream);
			for (const auto& provinceID : newContinentList.getInts()) continentMap.insert(std::make_pair(provinceID, continentName));
		});
}

std::optional<std::string> mappers::Continents::getEU4Continent(const int eu4Province) const
{
	const auto& mapping = continentMap.find(eu4Province);
	if (mapping != continentMap.end()) return mapping->second;
	return std::nullopt;
}