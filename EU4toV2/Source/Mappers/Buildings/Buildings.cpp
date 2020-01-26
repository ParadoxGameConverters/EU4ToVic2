#include "Buildings.h"
#include "ParserHelpers.h"
#include "../../Configuration.h"
#include "OSCompatibilityLayer.h"

mappers::Buildings::Buildings()
{
	registerKeys();

	if (Utils::doesFolderExist(theConfiguration.getEU4Path() + "/common/buildings"))
	{
		std::set<std::string> filenames;
		Utils::GetAllFilesInFolder(theConfiguration.getEU4Path() + "/common/buildings/", filenames);
		for (const auto& filename : filenames)
		{
			parseFile(theConfiguration.getEU4Path() + "/common/buildings/" + filename);
		}
	}
	for (const auto& modName : theConfiguration.getEU4Mods())
	{
		if (Utils::doesFolderExist(modName + "/common/buildings"))
		{
			std::set<std::string> filenames;
			Utils::GetAllFilesInFolder(modName + "/common/buildings/", filenames);
			for (const auto& filename : filenames)
			{
				parseFile(modName + "/common/buildings/" + filename);
			}
		}
	}
	clearRegisteredKeywords();
}

mappers::Buildings::Buildings(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::Buildings::registerKeys()
{
	registerRegex("[a-zA-Z0-9_]+", [this](const std::string& buildingName, std::istream& theStream) {
		Building building(theStream);
		buildings.insert(std::make_pair(buildingName, building));
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);
}

std::optional<mappers::Building> mappers::Buildings::getBuilding(const std::string& buildingName) const
{
	if (buildings.count(buildingName)) return std::make_optional(buildings.at(buildingName));
	return std::nullopt;
}