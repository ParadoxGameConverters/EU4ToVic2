#include "RegionLocalizations.h"
#include "../../Configuration.h"
#include "OSCompatibilityLayer.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include "Log.h"
namespace fs = std::filesystem;

mappers::RegionLocalizations::RegionLocalizations()
{
	LOG(LogLevel::Info) << "Parsing Region Localizations.";

	std::set<std::string> locFiles;
	Utils::GetAllFilesInFolder(theConfiguration.getEU4Path() + "/localisation/", locFiles);
	for (const auto& locFile : locFiles)
	{
		auto position = locFile.find("l_english");
		if (position != std::string::npos)
		{
			auto locMap = chopFile(locFile);
			engLocalisations.insert(locMap.begin(), locMap.end());
		}		
		position = locFile.find("l_french");
		if (position != std::string::npos)
		{
			auto locMap = chopFile(locFile);
			fraLocalisations.insert(locMap.begin(), locMap.end());
		}
		position = locFile.find("l_spanish");
		if (position != std::string::npos)
		{
			auto locMap = chopFile(locFile);
			spaLocalisations.insert(locMap.begin(), locMap.end());
		}
		position = locFile.find("l_german");
		if (position != std::string::npos)
		{
			auto locMap = chopFile(locFile);
			gerLocalisations.insert(locMap.begin(), locMap.end());
		}
	}
}

std::map<std::string, std::string> mappers::RegionLocalizations::chopFile(const std::string& locFile) const
{
	auto theFile = std::ifstream(fs::u8path(theConfiguration.getEU4Path() + "/localisation/" + locFile));
	if (!theFile.is_open()) throw std::runtime_error("Cannot open area/region localizations at " + theConfiguration.getEU4Path() + "/localisation/" + locFile);
	
	std::map<std::string, std::string> storedLocs;
	std::string line;
	// drop first line
	std::getline(theFile, line);
	
	while (std::getline(theFile, line))
	{
		std::istringstream iss(line);
		std::string keyblock, loc;
		
		// something went wrong
		if (!(iss >> keyblock >> loc)) break;
		
		// we need only adjectives
		const auto position = keyblock.find(":");
		if (position == std::string::npos) continue;

		auto key = keyblock.substr(0, position);
		storedLocs.insert(std::make_pair(key, loc));
	}
	return storedLocs;
}