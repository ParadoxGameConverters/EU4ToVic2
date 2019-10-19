/*Copyright (c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/



#include "Mods.h"
#include "Mod.h"
#include "../../Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <filesystem>
#include <fstream>



EU4::Mods::Mods(std::istream& theStream, Configuration& theConfiguration)
{
	std::set<std::string> usedMods;
	registerKeyword(std::regex("\".+\""), [&usedMods](const std::string& modName, std::istream& theStream) {
		if (modName.substr(0, 1) == "\"")
		{
			usedMods.insert(modName.substr(1, modName.size() - 2));
		}
		else
		{
			usedMods.insert(modName);
		}
	});

	parseStream(theStream);

	loadEU4ModDirectory(theConfiguration);
	loadSteamWorkshopDirectory(theConfiguration);
	loadCK2ExportDirectory(theConfiguration);

	Log(LogLevel::Debug) << "Finding Used Mods";
	for (auto usedMod: usedMods)
	{
		auto possibleModPath = getModPath(usedMod);
		if (possibleModPath)
		{
			if (!Utils::doesFolderExist(*possibleModPath) && !Utils::DoesFileExist(*possibleModPath))
			{
				LOG(LogLevel::Error) << usedMod << " could not be found in the specified mod directory - a valid mod directory must be specified. Tried " << *possibleModPath;
				exit(-1);
			}
			else
			{
				LOG(LogLevel::Debug) << "EU4 Mod is at " << *possibleModPath;
				theConfiguration.addEU4Mod(*possibleModPath);
			}
		}
		else
		{
			LOG(LogLevel::Error) << "No path could be found for " << usedMod << ". Check that the mod is present and that the .mod file specifies the path for the mod";
			exit(-1);
		}
	}
}


void EU4::Mods::loadEU4ModDirectory(const Configuration& theConfiguration)
{
	std::string EU4DocumentsLoc = theConfiguration.getEU4DocumentsPath();
	if (!Utils::doesFolderExist(EU4DocumentsLoc))
	{
		std::exception e("No Europa Universalis 4 documents directory was specified in configuration.txt, " \
			"or the path was invalid");
		throw e;
	}
	else
	{
		LOG(LogLevel::Debug) << "EU4 Documents directory is " << EU4DocumentsLoc;
		loadModDirectory(EU4DocumentsLoc);
	}
}


void EU4::Mods::loadSteamWorkshopDirectory(const Configuration& theConfiguration)
{
	std::string steamWorkshopPath = theConfiguration.getSteamWorkshopPath();
	if (!Utils::doesFolderExist(steamWorkshopPath))
	{
		std::exception e("No Steam Worksop directory was specified in configuration.txt, or the path was invalid");
		throw e;
	}
	else
	{
		LOG(LogLevel::Debug) << "Steam Workshop directory is " << steamWorkshopPath;
		for (auto& directoryItem: std::filesystem::directory_iterator(steamWorkshopPath))
		{
			std::filesystem::path path = directoryItem.path();
			auto fullPath = path.append("descriptor.mod");
			if (directoryItem.is_directory() && std::filesystem::exists(fullPath))
			{
				std::ifstream modFile(fullPath);
				Mod theMod(modFile);
				modFile.close();

				if (theMod.isValid())
				{
					possibleMods.insert(std::make_pair(theMod.getName(), directoryItem.path().generic_string()));
					Log(LogLevel::Debug) << "\tFound a mod named " << theMod.getName() << " at " << directoryItem.path();
				}
			}
		}
	}
}


void EU4::Mods::loadCK2ExportDirectory(const Configuration& theConfiguration)
{
	std::string CK2ExportLoc = theConfiguration.getCK2ExportPath();
	if (!Utils::doesFolderExist(CK2ExportLoc))
	{
		LOG(LogLevel::Warning) << "No Crusader Kings 2 mod directory was specified in configuration.txt," \
			" or the path was invalid - this will cause problems with CK2 converted saves";
	}
	else
	{
		LOG(LogLevel::Debug) << "CK2 export directory is " << CK2ExportLoc;
		loadModDirectory(CK2ExportLoc);
	}
}


void EU4::Mods::loadModDirectory(const std::string& directory)
{
	std::set<std::string> filenames;
	Utils::GetAllFilesInFolder(directory + "/mod", filenames);
	for (auto filename: filenames)
	{
		const int pos = filename.find_last_of('.');
		if ((pos != std::string::npos) && (filename.substr(pos, filename.length()) == ".mod"))
		{
			std::ifstream modFile(directory + "/mod/" + filename);
			Mod theMod(modFile);
			modFile.close();

			if (theMod.isValid())
			{
				std::string trimmedFilename = filename.substr(0, pos);

				possibleMods.insert(std::make_pair(theMod.getName(), directory + "/" + theMod.getPath()));
				possibleMods.insert(std::make_pair("mod/" + filename, directory + "/" + theMod.getPath()));
				possibleMods.insert(std::make_pair(trimmedFilename, directory + "/" + theMod.getPath()));
				Log(LogLevel::Debug) << "\tFound a mod named " << theMod.getName() << " with a mod file at " \
					<< "mod/" + filename << " claiming to be at " << directory << "/" << theMod.getPath();
			}
		}
	}
}


std::optional<std::string> EU4::Mods::getModPath(const std::string& modName) const
{
	auto mod = possibleMods.find(modName);
	if (mod == possibleMods.end())
	{
		return {};
	}
	else
	{
		return mod->second;
	}
}