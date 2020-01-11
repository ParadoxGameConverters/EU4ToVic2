#include "Modifiers.h"
#include "ParserHelpers.h"
#include "../../Configuration.h"
#include "OSCompatibilityLayer.h"
#include <set>
#include "Log.h"

EU4::Modifiers::Modifiers()
{
	registerKeys();
}

EU4::Modifiers::Modifiers(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU4::Modifiers::initialize()
{
	processFolder("event_modifiers");
	processFolder("triggered_modifiers");
	processFolder("static_modifiers");
	clearRegisteredKeywords();
}

void EU4::Modifiers::processFolder(const std::string& folderName)
{
	if (Utils::doesFolderExist(theConfiguration.getEU4Path() + "/common/" + folderName))
	{
		std::set<std::string> filenames;
		Utils::GetAllFilesInFolder(theConfiguration.getEU4Path() + "/common/" + folderName + "/", filenames);
		for (auto filename : filenames)
		{
			parseFile(theConfiguration.getEU4Path() + "/common/" + folderName + "/" + filename);
		}
	}
	for (auto modName : theConfiguration.getEU4Mods())
	{
		if (Utils::doesFolderExist(modName + "/common/" + folderName))
		{
			std::set<std::string> filenames;
			Utils::GetAllFilesInFolder(modName + "/common/" + folderName + "/", filenames);
			for (auto filename : filenames)
			{
				parseFile(modName + "/common/" + folderName + "/" + filename);
			}
		}
	}
}

void EU4::Modifiers::registerKeys()
{
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this](const std::string& modifierName, std::istream& theStream) {
		Modifier modifier(theStream);
		modifiers.insert(std::make_pair(modifierName, modifier));
	});
}

std::optional<EU4::Modifier> EU4::Modifiers::getModifier(const std::string& modifierName) const
{
	if (modifiers.count(modifierName) > 0) return std::make_optional(modifiers.at(modifierName));

	return std::nullopt;
}