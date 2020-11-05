#include "Mods.h"
#include "CommonFunctions.h"
#include "Configuration.h"
#include "Log.h"
#include "Mod.h"
#include "OSCompatibilityLayer.h"
#include <ZipFile.h>
#include <filesystem>
#include <fstream>
#include <set>
#include <stdexcept>
#include <string>
namespace fs = std::filesystem;

EU4::Mods::Mods(const std::vector<std::string>& usedMods)
{
	loadEU4ModDirectory();
	loadSteamWorkshopDirectory();
	loadCK2ExportDirectory();

	Log(LogLevel::Info) << "\tFinding Used Mods";
	for (const auto& usedMod: usedMods)
	{
		auto possibleModPath = getModPath(usedMod);
		if (possibleModPath)
		{
			if (!commonItems::DoesFolderExist(*possibleModPath) && !commonItems::DoesFileExist(*possibleModPath))
				throw std::invalid_argument(
					 usedMod + " could not be found in the specified mod directory, a valid mod directory must be specified. Tried: " + *possibleModPath);

			LOG(LogLevel::Info) << "\t\t->> Using EU4 Mod: " << *possibleModPath;
			theConfiguration.addEU4Mod(*possibleModPath);
		}
		else
		{
			throw std::invalid_argument("No path could be found for " + usedMod +
												 ". This means you have either removed/unsubscribed a mod the game was using, or that it cannot be found!");
		}
	}
}

void EU4::Mods::loadEU4ModDirectory()
{
	const auto& EU4DocumentsLoc = theConfiguration.getEU4DocumentsPath();
	if (!commonItems::DoesFolderExist(EU4DocumentsLoc))
		throw std::invalid_argument("No Europa Universalis 4 documents directory was specified in configuration.txt, or the path was invalid!");

	LOG(LogLevel::Info) << "\tEU4 Documents directory is " << EU4DocumentsLoc;
	loadModDirectory(EU4DocumentsLoc);
}

void EU4::Mods::loadSteamWorkshopDirectory()
{
	const auto& steamWorkshopPath = theConfiguration.getSteamWorkshopPath();
	if (!commonItems::DoesFolderExist(steamWorkshopPath))
		throw std::invalid_argument("No Steam Worksop directory was specified in configuration.txt, or the path was invalid");

	LOG(LogLevel::Info) << "\tSteam Workshop directory is " << steamWorkshopPath;
	auto subfolders = commonItems::GetAllSubfolders(steamWorkshopPath);
	for (const auto& subfolder: subfolders)
	{
		const auto path = steamWorkshopPath + "/" + subfolder;
		const auto descriptorFilename = path + "/descriptor.mod";
		if (commonItems::DoesFolderExist(path) && commonItems::DoesFileExist(descriptorFilename))
		{
			std::ifstream modFile(fs::u8path(path + "/descriptor.mod"));
			Mod theMod(modFile);
			modFile.close();

			if (theMod.isValid())
			{
				possibleMods.insert(std::make_pair("mod/ugc_" + subfolder + ".mod", path));
				possibleMods.insert(std::make_pair(theMod.getName(), path));
				Log(LogLevel::Info) << "\t\tFound potential mod named " << theMod.getName() << " at " << path;
			}
		}
	}
}


void EU4::Mods::loadCK2ExportDirectory()
{
	const auto CK2ExportLoc = theConfiguration.getCK2ExportPath();
	if (commonItems::DoesFolderExist(CK2ExportLoc))
	{
		LOG(LogLevel::Info) << "\tCK2 export directory is " << CK2ExportLoc;
		loadModDirectory(CK2ExportLoc);
	}
}


void EU4::Mods::loadModDirectory(const std::string& searchDirectory)
{
	for (const auto& filename: commonItems::GetAllFilesInFolder(searchDirectory + "/mod"))
	{
		if (getExtension(filename) == "mod")
		{
			try
			{
				std::ifstream modFile(fs::u8path(searchDirectory + "/mod/" + filename));
				Mod theMod(modFile);
				modFile.close();

				if (theMod.isValid())
				{
					if (!theMod.isCompressed())
					{
						const auto trimmedFilename = trimExtension(filename);

						std::string recordDirectory;
						if (commonItems::DoesFolderExist(theMod.getPath()))
						{
							recordDirectory = theMod.getPath();
						}
						else if (commonItems::DoesFolderExist(searchDirectory + "/" + theMod.getPath()))
						{
							recordDirectory = searchDirectory + "/" + theMod.getPath();
						}
						else
						{
							throw std::invalid_argument("");
						}

						possibleMods.insert(std::make_pair(theMod.getName(), recordDirectory));
						possibleMods.insert(std::make_pair("mod/" + filename, recordDirectory));
						possibleMods.insert(std::make_pair(trimmedFilename, recordDirectory));
						Log(LogLevel::Info) << "\t\tFound potential mod named " << theMod.getName() << " with a mod file at " << searchDirectory << "/mod/" + filename
												  << " and itself at " << recordDirectory;
					}
					else
					{
						std::string recordDirectory;
						if (commonItems::DoesFileExist(theMod.getPath()))
						{
							recordDirectory = theMod.getPath();
						}
						else if (commonItems::DoesFileExist(searchDirectory + "/" + theMod.getPath()))
						{
							recordDirectory = searchDirectory + "/" + theMod.getPath();
						}
						else
						{
							throw std::invalid_argument("");
						}

						const auto trimmedFilename = trimExtension(filename);

						possibleCompressedMods.insert(std::make_pair(theMod.getName(), recordDirectory));
						possibleCompressedMods.insert(std::make_pair("mod/" + filename, recordDirectory));
						possibleCompressedMods.insert(std::make_pair(trimmedFilename, recordDirectory));
						Log(LogLevel::Info) << "\t\tFound a compressed mod named " << theMod.getName() << " with a mod file at " << searchDirectory
												  << "/mod/" + filename << " and itself at " << recordDirectory;
					}
				}
			}
			catch (std::exception&)
			{
				LOG(LogLevel::Warning) << "Error while reading " << searchDirectory << "/mod/" << filename
											  << ". "
												  "Mod will not be useable for conversions.";
			}
		}
	}
}


std::optional<std::string> EU4::Mods::getModPath(const std::string& modName) const
{
	const auto mod = possibleMods.find(modName);
	if (mod != possibleMods.end())
	{
		return mod->second;
	}

	const auto compressedMod = possibleCompressedMods.find(modName);
	if (compressedMod != possibleCompressedMods.end())
	{
		const auto archivePath = compressedMod->second;
		const auto uncompressedName = trimPath(trimExtension(archivePath));

		commonItems::TryCreateFolder("mods/");

		if (!commonItems::DoesFolderExist("mods/" + uncompressedName))
		{
			LOG(LogLevel::Info) << "\t\tUncompressing: " << archivePath;
			if (!extractZip(archivePath, "mods/" + uncompressedName))
			{
				LOG(LogLevel::Warning) << "We have trouble automatically uncompressing your mod.";
				LOG(LogLevel::Warning) << "Please, manually uncompress: " << archivePath;
				LOG(LogLevel::Warning) << "Into: EU4ToVic2/mod/" << uncompressedName;
				LOG(LogLevel::Warning) << "Then run the converter again. Thank you and good luck.";
				return std::nullopt;
			}
		}

		if (commonItems::DoesFolderExist("mods/" + uncompressedName))
		{
			return "mods/" + uncompressedName;
		}
	}

	return std::nullopt;
}

bool EU4::Mods::extractZip(const std::string& archive, const std::string& path) const
{
	commonItems::TryCreateFolder(path);
	const auto modfile = ZipFile::Open(archive);
	if (!modfile)
		return false;
	for (size_t entryNum = 0; entryNum < modfile->GetEntriesCount(); ++entryNum)
	{
		const auto& entry = modfile->GetEntry(static_cast<int>(entryNum));
		const auto& inPath = entry->GetFullName();
		const auto& name = entry->GetName();
		if (entry->IsDirectory())
			continue;

		// Does target directory exist?
		const auto dirNamePos = inPath.find(name);
		const auto dirName = path + "/" + inPath.substr(0, dirNamePos);
		if (!commonItems::DoesFolderExist(dirName))
		{
			// we need to craft our way through to target directory.
			auto remainder = inPath;
			auto currentPath = path;
			while (remainder != name)
			{
				const auto pos = remainder.find_first_of('/');
				if (pos != std::string::npos)
				{
					const auto makeDirName = remainder.substr(0, pos);
					currentPath += "/" + makeDirName;
					commonItems::TryCreateFolder(currentPath);
					remainder = remainder.substr(pos + 1, remainder.length());
				}
				else
					break;
			}
		}
		ZipFile::ExtractFile(archive, inPath, path + "/" + inPath);
	}
	return true;
}