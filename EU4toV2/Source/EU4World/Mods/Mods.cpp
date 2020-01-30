#include "Mods.h"
#include "Mod.h"
#include "../../Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <fstream>
#include <set>
#include <stdexcept>
#include <string>
#include <filesystem>
#include "../../../../ZipLib/ZipFile.h"

namespace fs = std::filesystem;

EU4::Mods::Mods(const std::vector<std::string>& usedMods, Configuration& theConfiguration)
{
	loadEU4ModDirectory(theConfiguration);
	loadSteamWorkshopDirectory(theConfiguration);
	loadCK2ExportDirectory(theConfiguration);
	
	Log(LogLevel::Info) << "\tFinding Used Mods";
	for (const auto& usedMod: usedMods)
	{
		auto possibleModPath = getModPath(usedMod);
		if (possibleModPath)
		{
			if (!Utils::doesFolderExist(*possibleModPath) && !Utils::DoesFileExist(*possibleModPath))
				throw std::invalid_argument(usedMod + " could not be found in the specified mod directory " + \
					"- a valid mod directory must be specified. Tried " + *possibleModPath);

			LOG(LogLevel::Info) << "\t\t->> Using EU4 Mod: " << *possibleModPath;
			theConfiguration.addEU4Mod(*possibleModPath);
		}
		else
		{
			throw std::invalid_argument("No path could be found for " + usedMod + ". Check that the mod is present and that the .mod file specifies the path for the mod");
		}
	}
}

void EU4::Mods::loadEU4ModDirectory(const Configuration& theConfiguration)
{
	const auto EU4DocumentsLoc = theConfiguration.getEU4DocumentsPath();
	if (!Utils::doesFolderExist(EU4DocumentsLoc))
		throw std::invalid_argument("No Europa Universalis 4 documents directory was specified in configuration.txt, or the path was invalid!");

	LOG(LogLevel::Info) << "\tEU4 Documents directory is " << EU4DocumentsLoc;
	loadModDirectory(EU4DocumentsLoc);
}

void EU4::Mods::loadSteamWorkshopDirectory(const Configuration& theConfiguration)
{
	const auto steamWorkshopPath = theConfiguration.getSteamWorkshopPath();
	if (!Utils::doesFolderExist(steamWorkshopPath))
		throw std::invalid_argument("No Steam Worksop directory was specified in configuration.txt, or the path was invalid");

	LOG(LogLevel::Info) << "\tSteam Workshop directory is " << steamWorkshopPath;
	std::set<std::string> subfolders;
	Utils::GetAllSubfolders(steamWorkshopPath, subfolders);
	for (const auto& subfolder: subfolders)
	{
		const auto path = steamWorkshopPath + "/" + subfolder;
		const auto descriptorFilename = path + "/descriptor.mod";
		if (Utils::doesFolderExist(path) && Utils::DoesFileExist(descriptorFilename))
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


void EU4::Mods::loadCK2ExportDirectory(const Configuration& theConfiguration)
{
	const auto CK2ExportLoc = theConfiguration.getCK2ExportPath();
	if (!Utils::doesFolderExist(CK2ExportLoc))
	{
		LOG(LogLevel::Warning) << "No Crusader Kings 2 mod directory was specified in configuration.txt," \
			" or the path was invalid - this will cause problems with CK2 converted saves";
	}
	else
	{
		LOG(LogLevel::Info) << "\tCK2 export directory is " << CK2ExportLoc;
		loadModDirectory(CK2ExportLoc);
	}
}


void EU4::Mods::loadModDirectory(const std::string& searchDirectory)
{
	std::set<std::string> filenames;
	Utils::GetAllFilesInFolder(searchDirectory + "/mod", filenames);
	for (const auto& filename: filenames)
	{
		const auto pos = filename.find_last_of('.');
		if (pos != std::string::npos && filename.substr(pos, filename.length()) == ".mod")
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
						const auto trimmedFilename = filename.substr(0, pos);

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
							throw std::invalid_argument("");
						}

						possibleMods.insert(std::make_pair(theMod.getName(), recordDirectory));
						possibleMods.insert(std::make_pair("mod/" + filename, recordDirectory));
						possibleMods.insert(std::make_pair(trimmedFilename, recordDirectory));
						Log(LogLevel::Info) << "\t\tFound potential mod named " << theMod.getName() <<
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
							throw std::invalid_argument("");
						}

						const auto trimmedFilename = filename.substr(0, pos);

						possibleCompressedMods.insert(std::make_pair(theMod.getName(), recordDirectory));
						possibleCompressedMods.insert(std::make_pair("mod/" + filename, recordDirectory));
						possibleCompressedMods.insert(std::make_pair(trimmedFilename, recordDirectory));
						Log(LogLevel::Info) << "\tFound a compessed mod named " << theMod.getName() <<
							" with a mod file at " << searchDirectory << "/mod/" + filename <<
							" and itself at " << recordDirectory;
					}
				}
			}
			catch (std::exception&)
			{
				LOG(LogLevel::Warning) << "Error while reading " << searchDirectory << "/mod/" << filename << ". " \
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
		auto uncompressedName = archivePath.substr(0, archivePath.find_last_of('.'));
		auto pos = uncompressedName.find_last_of('\\');
		if (pos != std::string::npos)
		{
			uncompressedName = uncompressedName.substr(pos + 1, uncompressedName.size());
		}
		pos = uncompressedName.find_last_of('/');
		if (pos != std::string::npos)
		{
			uncompressedName = uncompressedName.substr(pos + 1, uncompressedName.size());
		}

		if (!Utils::doesFolderExist("mods/")) fs::create_directory("mods/");		

		if (!Utils::doesFolderExist("mods/" + uncompressedName))
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

		if (Utils::doesFolderExist("mods/" + uncompressedName))
		{
			return "mods/" + uncompressedName;
		}

	}

	return std::nullopt;
}

bool EU4::Mods::extractZip(const std::string& archive, const std::string& path) const
{
	fs::create_directory(path);
	auto modfile = ZipFile::Open(archive);
	if (!modfile) return false;
	for (size_t entryNum = 0; entryNum < modfile->GetEntriesCount(); ++entryNum)
	{
		const auto& entry = modfile->GetEntry(entryNum);
		const auto& inpath = entry->GetFullName();
		const auto& name = entry->GetName();
		if (entry->IsDirectory()) continue;

		// Does target directory exist?
		const auto dirnamepos = inpath.find(name);
		const auto dirname = path + "/" + inpath.substr(0, dirnamepos);
		if (!exists(fs::u8path(dirname)))
		{
			// we need to craft our way through to target directory.
			auto remainder = inpath;
			auto currentpath = path;
			while (remainder != name)
			{
				const auto pos = remainder.find_first_of('/');
				if (pos != std::string::npos)
				{
					auto makedirname = remainder.substr(0, pos);
					currentpath += "/" + makedirname;
					create_directory(fs::u8path(currentpath));
					remainder = remainder.substr(pos + 1, remainder.length());
				}
				else break;
			}
		}
		ZipFile::ExtractFile(archive, inpath, path + "/" + inpath);
	}
	return true;
}