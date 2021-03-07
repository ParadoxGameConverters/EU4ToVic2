#include "StateNameParser.h"
#include "Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

V2::StateNameParser::StateNameParser()
{
	if (commonItems::DoesFileExist("./blankMod/output/localisation/text.csv"))
	{
		importStateLocalizations("./blankMod/output/localisation/text.csv");
	}
	const auto& stateLocPath = theConfiguration.getVic2Path() + "/localisation";
	if (commonItems::DoesFolderExist(stateLocPath))
	{
		const auto& locFiles = commonItems::GetAllFilesInFolderRecursive(stateLocPath);
		for (const auto& locFile: locFiles)
		{
			importStateLocalizations(stateLocPath + "/" + locFile);
		}
	}
}

V2::StateNameParser::StateNameParser(std::string folder)
{
	if (commonItems::DoesFolderExist(folder))
	{
		const auto& locFiles = commonItems::GetAllFilesInFolderRecursive(folder);
		for (const auto& locFile: locFiles)
		{
			importStateLocalizations(folder + "/" + locFile);
		}
	}
}

void V2::StateNameParser::importStateLocalizations(const std::string& file)
{
	std::ifstream locFile(fs::u8path(file));

	while (locFile.good() && !locFile.eof())
	{
		std::regex state("^([A-Z]{3}_[0-9]+);");
		std::smatch sm;
		std::string line;
		getline(locFile, line);

		std::string stateID;
		if (std::regex_search(line, sm, state))
		{
			stateID = sm[1];
		}
		else
		{
			continue;
		}

		const auto position = line.find_first_of(';');
		const auto name = line.substr(position + 1, line.find_first_of(';', position + 1) - position - 1);

		if (stateNames.find(stateID) == stateNames.end())
		{
			stateNames.insert(std::make_pair(stateID, name));
		}
	}
	locFile.close();
}

std::optional<std::string> V2::StateNameParser::getStateName(const std::string& stateID) const
{
	const auto& name = stateNames.find(stateID);
	if (name != stateNames.end()) return name->second;
	return std::nullopt;
}
