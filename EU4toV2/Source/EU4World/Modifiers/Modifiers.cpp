#include "Modifiers.h"
#include "Configuration.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

EU4::Modifiers::Modifiers(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

EU4::Modifiers::Modifiers()
{
	registerKeys();
	processFolder("event_modifiers");
	processFolder("triggered_modifiers");
	processFolder("static_modifiers");
	clearRegisteredKeywords();
}

void EU4::Modifiers::processFolder(const std::string& folderName)
{
	if (commonItems::DoesFolderExist(theConfiguration.getEU4Path() + "/common/" + folderName))
		for (const auto& filename: commonItems::GetAllFilesInFolder(theConfiguration.getEU4Path() + "/common/" + folderName + "/"))
			parseFile(theConfiguration.getEU4Path() + "/common/" + folderName + "/" + filename);

	for (const auto& modName: theConfiguration.getEU4Mods())
		for (const auto& filename: commonItems::GetAllFilesInFolder(modName + "/common/" + folderName + "/"))
			parseFile(modName + "/common/" + folderName + "/" + filename);
}

void EU4::Modifiers::registerKeys()
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& modifierName, std::istream& theStream) {
		Modifier modifier(theStream);
		modifiers.insert(std::make_pair(modifierName, modifier));
	});
}

std::optional<EU4::Modifier> EU4::Modifiers::getModifier(const std::string& modifierName) const
{
	if (modifiers.count(modifierName))
		return modifiers.at(modifierName);
	else
		return std::nullopt;
}