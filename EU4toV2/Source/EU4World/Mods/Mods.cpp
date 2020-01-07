#include "Mods.h"
#include "Mod.h"
#include "../../Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <fstream>
#include <set>
#include <stdexcept>
#include <string>

EU4::Mods::Mods(std::istream& theStream, Configuration& theConfiguration)
{
	std::set<std::string> usedMods;
	registerKeyword(std::regex(".+"), [&usedMods](const std::string& modName, std::istream& theStream) {
		usedMods.insert(modName);
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
				LOG(LogLevel::Error) << usedMod << " could not be found in the specified mod directory " \
					"- a valid mod directory must be specified. Tried " << *possibleModPath;
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
		std::invalid_argument e(
			"No Europa Universalis 4 documents directory was specified in configuration.txt, or the path was invalid"
		);
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
		std::invalid_argument e(
			"No Steam Worksop directory was specified in configuration.txt, or the path was invalid"
		);
		throw e;
	}
	else
	{
		LOG(LogLevel::Debug) << "Steam Workshop directory is " << steamWorkshopPath;
		std::set<std::string> subfolders;
		Utils::GetAllSubfolders(steamWorkshopPath, subfolders);
		for (auto subfolder: subfolders)
		{
			std::string path = steamWorkshopPath + "/" + subfolder;
			std::string descriptorFilename = path + "/descriptor.mod";
			if (Utils::doesFolderExist(path) && Utils::DoesFileExist(descriptorFilename))
			{
				std::ifstream modFile(path + "/descriptor.mod");
				Mod theMod(modFile);
				modFile.close();

				if (theMod.isValid())
				{
					possibleMods.insert(std::make_pair("mod/ugc_" + subfolder + ".mod", path));
					possibleMods.insert(std::make_pair(theMod.getName(), path));
					Log(LogLevel::Debug) << "\tFound a mod named " << theMod.getName() << " at " << path;
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


void EU4::Mods::loadModDirectory(const std::string& searchDirectory)
{
	std::set<std::string> filenames;
	Utils::GetAllFilesInFolder(searchDirectory + "/mod", filenames);
	for (auto filename: filenames)
	{
		const int pos = filename.find_last_of('.');
		if ((pos != std::string::npos) && (filename.substr(pos, filename.length()) == ".mod"))
		{
			try
			{
				std::ifstream modFile(searchDirectory + "/mod/" + filename);
				Mod theMod(modFile);
				modFile.close();

				if (theMod.isValid())
				{
					if (!theMod.isCompressed())
					{
						std::string trimmedFilename = filename.substr(0, pos);

						std::string recordDirectory;
						if (Utils::doesFolderExist(theMod.getPath()))
						{
							recordDirectory = theMod.getPath();
						}
						else if (Utils::doesFolderExist(searchDirectory + "/" + theMod.getPath()))
						{
							recordDirectory = searchDirectory + "/" + theMod.getPath();
						}
						else
						{
							std::invalid_argument e("");
							throw e;
						}

						possibleMods.insert(std::make_pair(theMod.getName(), recordDirectory));
						possibleMods.insert(std::make_pair("mod/" + filename, recordDirectory));
						possibleMods.insert(std::make_pair(trimmedFilename, recordDirectory));
						Log(LogLevel::Debug) << "\tFound a mod named " << theMod.getName() <<
							" with a mod file at " << searchDirectory << "/mod/" + filename <<
							" and itself at " << recordDirectory;
					}
					else
					{
						std::string recordDirectory;
						if (Utils::DoesFileExist(theMod.getPath()))
						{
							recordDirectory = theMod.getPath();
						}
						else if (Utils::DoesFileExist(searchDirectory + "/" + theMod.getPath()))
						{
							recordDirectory = searchDirectory + "/" + theMod.getPath();
						}
						else
						{
							std::invalid_argument e("");
							throw e;
						}

						std::string trimmedFilename = filename.substr(0, pos);

						possibleCompressedMods.insert(std::make_pair(theMod.getName(), recordDirectory));
						possibleCompressedMods.insert(std::make_pair("mod/" + filename, recordDirectory));
						possibleCompressedMods.insert(std::make_pair(trimmedFilename, recordDirectory));
						Log(LogLevel::Debug) << "\tFound a compessed mod named " << theMod.getName() <<
							" with a mod file at " << searchDirectory << "/mod/" + filename <<
							" and itself at " << recordDirectory;
					}
				}
			}
			catch (std::exception e)
			{
				LOG(LogLevel::Warning) << "Error while reading " << searchDirectory << "/mod/" << filename << ". " \
					"Mod will not be useable for conversions.";
			}
		}
	}
}


std::optional<std::string> EU4::Mods::getModPath(const std::string& modName) const
{
	auto mod = possibleMods.find(modName);
	if (mod != possibleMods.end())
	{
		return mod->second;
	}

	auto compressedMod = possibleCompressedMods.find(modName);
	if (compressedMod != possibleCompressedMods.end())
	{
		std::string archivePath = compressedMod->second;
		std::string uncompressedName = archivePath.substr(0, archivePath.find_last_of('.'));
		int pos = uncompressedName.find_last_of('\\');
		if (pos != std::string::npos)
		{
			uncompressedName = uncompressedName.substr(pos + 1, uncompressedName.size());
		}
		pos = uncompressedName.find_last_of('/');
		if (pos != std::string::npos)
		{
			uncompressedName = uncompressedName.substr(pos + 1, uncompressedName.size());
		}

		if (Utils::doesFolderExist("mods/" + uncompressedName))
		{
			return "mods/" + uncompressedName;
		}
		else
		{
			LOG(LogLevel::Warning) << "For now, manually uncompress " << archivePath << \
				" into the converter directory at EU4ToVic2/mods/" << uncompressedName;
			LOG(LogLevel::Warning) << "Then run the converter again";
		}
	}

	return {};
}