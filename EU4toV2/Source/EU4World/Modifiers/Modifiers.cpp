#include "Modifiers.h"
#include "../../Configuration.h"
#include "OSCompatibilityLayer.h"
#include <set>

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
	if (Utils::DoesFolderExist(theConfiguration.getEU4Path() + "/common/" + folderName))
	{
		auto filenames = Utils::GetAllFilesInFolder(theConfiguration.getEU4Path() + "/common/" + folderName + "/");
		for (const auto& filename : filenames)
		{
			parseFile(theConfiguration.getEU4Path() + "/common/" + folderName + "/" + filename);
		}
	}
	for (const auto& modName : theConfiguration.getEU4Mods())
	{
		if (Utils::DoesFolderExist(modName + "/common/" + folderName))
		{
			auto filenames = Utils::GetAllFilesInFolder(modName + "/common/" + folderName + "/");
			for (const auto& filename : filenames)
			{
				parseFile(modName + "/common/" + folderName + "/" + filename);
			}
		}
	}
}

void EU4::Modifiers::registerKeys()
{
	registerRegex("[a-zA-Z0-9_]+", [this](const std::string& modifierName, std::istream& theStream) {
		Modifier modifier(theStream);
		modifiers.insert(std::make_pair(modifierName, modifier));
	});
}

std::optional<EU4::Modifier> EU4::Modifiers::getModifier(const std::string& modifierName) const
{
	if (modifiers.count(modifierName) > 0) return std::make_optional(modifiers.at(modifierName));

	return std::nullopt;
}