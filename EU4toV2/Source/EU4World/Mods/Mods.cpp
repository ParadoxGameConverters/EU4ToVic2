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
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <fstream>



EU4::Mods::Mods(const Configuration& theConfiguration)
{
	possibleMods.merge(loadEU4ModDirectory(theConfiguration));
	possibleMods.merge(loadCK2ExportDirectory(theConfiguration));
}


std::map<std::string, std::string> EU4::Mods::loadEU4ModDirectory(const Configuration& theConfiguration) const
{
	std::map<std::string, std::string> possibleMods;
	LOG(LogLevel::Debug) << "Get EU4 Mod Directory";
	std::string EU4DocumentsLoc = theConfiguration.getEU4DocumentsPath();
	if (!Utils::doesFolderExist(EU4DocumentsLoc))
	{
		LOG(LogLevel::Error) << "No Europa Universalis 4 documents directory was specified in configuration.txt," \
			"or the path was invalid";
		exit(-1);
	}
	else
	{
		LOG(LogLevel::Debug) << "EU4 Documents directory is " << EU4DocumentsLoc;
		std::set<std::string> filenames;
		Utils::GetAllFilesInFolder(EU4DocumentsLoc + "/mod", filenames);
		for (auto filename : filenames)
		{
			const int lastPeriod = filename.find_last_of('.');
			if (filename.substr(lastPeriod, filename.length()) == ".mod")
			{
				std::ifstream modFile(EU4DocumentsLoc + "/mod/" + filename);
				EU4::Mod theMod(modFile);
				modFile.close();

				if (theMod.isValid())
				{
					possibleMods.insert(std::make_pair(theMod.getName(), EU4DocumentsLoc + "/" + theMod.getPath()));
					Log(LogLevel::Debug) << "\tFound a mod named " << theMod.getName() << " claiming to be at " \
						<< EU4DocumentsLoc << "/" << theMod.getPath();
				}
			}
		}
	}

	return possibleMods;
}


std::map<std::string, std::string> EU4::Mods::loadCK2ExportDirectory(const Configuration& theConfiguration) const
{
	std::map<std::string, std::string> possibleMods;
	LOG(LogLevel::Debug) << "Get CK2 Export Directory";
	std::string CK2ExportLoc = theConfiguration.getCK2ExportPath();
	if (!Utils::doesFolderExist(CK2ExportLoc))
	{
		LOG(LogLevel::Warning) << "No Crusader Kings 2 mod directory was specified in configuration.txt," \
			" or the path was invalid - this will cause problems with CK2 converted saves";
	}
	else
	{
		LOG(LogLevel::Debug) << "CK2 export directory is " << CK2ExportLoc;
		std::set<std::string> filenames;
		Utils::GetAllFilesInFolder(CK2ExportLoc, filenames);
		for (auto filename : filenames)
		{
			const int pos = filename.find_last_of('.');
			if ((pos != std::string::npos) && (filename.substr(pos, filename.length()) == ".mod"))
			{
				std::ifstream modFile(CK2ExportLoc + "/" + filename);
				Mod theMod(modFile);
				modFile.close();

				if (theMod.isValid())
				{
					possibleMods.insert(std::make_pair(theMod.getName(), CK2ExportLoc + "/" + theMod.getPath()));
					Log(LogLevel::Debug) << "\tFound a mod named " << theMod.getName() << " claiming to be at " \
						<< CK2ExportLoc << "/" << theMod.getPath();
				}
			}
		}
	}

	return possibleMods;
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