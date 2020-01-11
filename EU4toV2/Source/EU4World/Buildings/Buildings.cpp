#include "Buildings.h"
#include "ParserHelpers.h"
#include "../../Configuration.h"
#include "OSCompatibilityLayer.h"

EU4::Buildings::Buildings()
{
	registerKeys();

	if (Utils::doesFolderExist(theConfiguration.getEU4Path() + "/common/buildings"))
	{
		std::set<std::string> filenames;
		Utils::GetAllFilesInFolder(theConfiguration.getEU4Path() + "/common/buildings/", filenames);
		for (auto filename : filenames)
		{
			parseFile(theConfiguration.getEU4Path() + "/common/buildings/" + filename);
		}
	}
	for (auto modName : theConfiguration.getEU4Mods())
	{
		if (Utils::doesFolderExist(modName + "/common/buildings"))
		{
			std::set<std::string> filenames;
			Utils::GetAllFilesInFolder(modName + "/common/buildings/", filenames);
			for (auto filename : filenames)
			{
				parseFile(modName + "/common/buildings/" + filename);
			}
		}
	}
	clearRegisteredKeywords();
}

EU4::Buildings::Buildings(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU4::Buildings::registerKeys()
{
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this](const std::string& buildingName, std::istream& theStream) {
		Building building(theStream);
		buildings.insert(std::make_pair(buildingName, building));
		});
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);
}


std::optional<EU4::Building> EU4::Buildings::getBuilding(const std::string& buildingName) const
{
	if (buildings.count(buildingName) > 0)
	{
		return std::make_optional(buildings.at(buildingName));
	}
	else
	{
		return std::nullopt;
	}
}