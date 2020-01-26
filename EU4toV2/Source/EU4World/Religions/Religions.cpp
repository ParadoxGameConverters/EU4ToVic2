#include "Religions.h"
#include "ReligionGroup.h"
#include "ParserHelpers.h"
#include "../../Configuration.h"
#include "OSCompatibilityLayer.h"
#include "Log.h"

EU4::Religions::Religions()
{
	LOG(LogLevel::Info) << "Parsing EU4 religions";

	registerKeys();

	std::set<std::string> filenames;
	Utils::GetAllFilesInFolder(theConfiguration.getEU4Path() + "/common/religions/", filenames);
	for (const auto& filename : filenames)
	{
		parseFile(theConfiguration.getEU4Path() + "/common/religions/" + filename);
	}
	for (const auto& modName : theConfiguration.getEU4Mods())
	{
		std::set<std::string> moreFilenames;
		Utils::GetAllFilesInFolder(modName + "/common/religions/", moreFilenames);
		for (const auto& filename : filenames)
		{
			parseFile(modName + "/common/religions/" + filename);
		}
	}
	clearRegisteredKeywords();
}

EU4::Religions::Religions(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU4::Religions::registerKeys()
{
	registerRegex("[a-zA-Z_]+", [this](const std::string& groupName, std::istream& theStream) 
		{
			ReligionGroup newGroup(groupName, theStream);
			for (const auto& religion : newGroup.takeReligions()) theReligions.insert(religion);
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);
}

std::optional<EU4::Religion> EU4::Religions::getReligion(const std::string& name) const
{
	const auto& religionsItr = theReligions.find(name);
	if (religionsItr != theReligions.end()) return religionsItr->second;
	return std::nullopt;
}