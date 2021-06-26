#include "Configuration.h"
#include "CommonFunctions.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include <fstream>

Configuration theConfiguration;

void Configuration::instantiate(std::istream& theStream,
	 const mappers::ConverterVersion& converterVersion,
	 bool (*DoesFolderExist)(const std::string& path2),
	 bool (*doesFileExist)(const std::string& path3))
{
	registerKeyword("SaveGame", [this](std::istream& theStream) {
		EU4SaveGamePath = commonItems::getString(theStream);
		LOG(LogLevel::Info) << "EU4 savegame path: " << EU4SaveGamePath;
	});
	registerKeyword("EU4directory", [this, converterVersion, DoesFolderExist, doesFileExist](std::istream& theStream) {
		EU4Path = commonItems::getString(theStream);
		verifyEU4Path(EU4Path, DoesFolderExist, doesFileExist);
		LOG(LogLevel::Info) << "EU4 path: " << EU4Path;
		verifyEU4Version(converterVersion);
	});
	registerKeyword("EU4DocumentsDirectory", [this](std::istream& theStream) {
		EU4DocumentsPath = commonItems::getString(theStream);
		LOG(LogLevel::Info) << "EU4 documents path: " << EU4DocumentsPath;
	});
	registerKeyword("SteamWorkshopDirectory", [this](std::istream& theStream) {
		SteamWorkshopPath = commonItems::getString(theStream);
		LOG(LogLevel::Info) << "EU4 steam workshop path: " << SteamWorkshopPath;
	});
	registerKeyword("CK2ExportDirectory", [this](std::istream& theStream) {
		CK2ExportPath = commonItems::getString(theStream);
		LOG(LogLevel::Info) << "CK2 Export path: " << CK2ExportPath;
	});
	registerKeyword("Vic2directory", [this, converterVersion, DoesFolderExist, doesFileExist](std::istream& theStream) {
		Vic2Path = commonItems::getString(theStream);
		verifyVic2Path(Vic2Path, DoesFolderExist, doesFileExist);
		LOG(LogLevel::Info) << "Vic2 path: " << Vic2Path;
		verifyVic2Version(converterVersion);
	});
	registerKeyword("Vic2Documentsdirectory", [this, DoesFolderExist](std::istream& theStream) {
		Vic2DocumentsPath = commonItems::getString(theStream);
		verifyVic2DocumentsPath(Vic2DocumentsPath, DoesFolderExist);
		LOG(LogLevel::Info) << "Vic2 documents path: " << Vic2DocumentsPath;
	});
	registerKeyword("max_literacy", [this](std::istream& theStream) {
		const auto maxLiteracyString = commonItems::getString(theStream);
		MaxLiteracy = static_cast<double>(std::stoi(maxLiteracyString)) / 100;
		LOG(LogLevel::Info) << "Max Literacy: " << MaxLiteracy;
	});
	registerKeyword("remove_type", [this](std::istream& theStream) {
		const auto removeTypeString = commonItems::getString(theStream);
		removeType = DEADCORES(std::stoi(removeTypeString));
	});
	registerKeyword("absorb_colonies", [this](std::istream& theStream) {
		const auto absorbString = commonItems::getString(theStream);
		absorbColonies = ABSORBCOLONIES(std::stoi(absorbString));
		LOG(LogLevel::Info) << "Absorb Colonies: " << absorbString;
	});
	registerKeyword("liberty_threshold", [this](std::istream& theStream) {
		const auto libertyThresholdString = commonItems::getString(theStream);
		libertyThreshold = LIBERTYDESIRE(std::stoi(libertyThresholdString));
		LOG(LogLevel::Info) << "Liberty Treshold: " << libertyThresholdString;
	});
	registerKeyword("pop_shaping", [this](std::istream& theStream) {
		const auto popShapingString = commonItems::getString(theStream);
		popShaping = POPSHAPES(std::stoi(popShapingString));
		LOG(LogLevel::Info) << "Pop Shaping: " << popShapingString;
	});
	registerKeyword("core_handling", [this](std::istream& theStream) {
		const auto coreHandlingString = commonItems::getString(theStream);
		coreHandling = COREHANDLES(std::stoi(coreHandlingString));
		LOG(LogLevel::Info) << "Core Handling: " << coreHandlingString;
	});
	registerKeyword("pop_shaping_factor", [this](std::istream& theStream) {
		const auto popShapingFactorString = commonItems::getString(theStream);
		popShapingFactor = static_cast<double>(std::stoi(popShapingFactorString));
		LOG(LogLevel::Info) << "Pop Shaping Factor: " << popShapingFactor;
	});
	registerKeyword("euro_centrism", [this](std::istream& theStream) {
		const auto euroCentrismString = commonItems::getString(theStream);
		euroCentric = EUROCENTRISM(std::stoi(euroCentrismString));
		LOG(LogLevel::Info) << "Eurocentrism: " << euroCentrismString;
	});
	registerKeyword("africa_reset", [this](std::istream& theStream) {
		const auto africaResetString = commonItems::getString(theStream);
		africaReset = AFRICARESET(std::stoi(africaResetString));
		LOG(LogLevel::Info) << "Africa Reset: " << africaResetString;
	});
	registerKeyword("debug", [this](std::istream& theStream) {
		const auto debugString = commonItems::getString(theStream);
		debug = (debugString == "yes");
	});
	registerKeyword("randomise_rgos", [this](std::istream& theStream) {
		const auto randomiseRgosString = commonItems::getString(theStream);
		randomiseRgos = randomiseRgosString == "yes";
		LOG(LogLevel::Info) << "Randomise RGOs: " << randomiseRgosString;
	});
	registerKeyword("convert_all", [this](std::istream& theStream) {
		const auto convertAllString = commonItems::getString(theStream);
		convertAll = convertAllString == "yes";
		LOG(LogLevel::Info) << "Convert All: " << convertAllString;
	});
	registerKeyword("hybrid_mod", [this](std::istream& theStream) {
		const auto hybridModString = commonItems::getString(theStream);
		hybridMod = HYBRIDMOD(std::stoi(hybridModString));
		LOG(LogLevel::Info) << "Hybrid mod: " << hybridModString;
	});
	registerKeyword("output_name", [this](std::istream& theStream) {
		incomingOutputName = commonItems::getString(theStream);
		LOG(LogLevel::Info) << "Output Name: " << incomingOutputName;
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	LOG(LogLevel::Info) << "Reading configuration file";
	parseStream(theStream);
	clearRegisteredKeywords();
	setOutputName();
	if (isHpmEnabled())
	{
		if (!DoesFolderExist(Vic2Path + "/mod/HPM"))
			throw std::runtime_error(Vic2Path + "/mod/HPM does not exist!");
		Vic2Path += "/mod/HPM";
	}
	Log(LogLevel::Progress) << "3 %";
}

void Configuration::verifyEU4Path(const std::string& path, bool (*DoesFolderExist)(const std::string& path2), bool (*doesFileExist)(const std::string& path3))
{
	if (!DoesFolderExist(path))
		throw std::runtime_error(path + " does not exist!");
	if (!doesFileExist(path + "/eu4.exe") && !doesFileExist(path + "/eu4") && !DoesFolderExist(path + "/eu4.app"))
		throw std::runtime_error(path + " does not contain Europa Universalis 4!");
	if (!doesFileExist(path + "/map/positions.txt"))
		throw std::runtime_error(path + " does not appear to be a valid EU4 install!");
}

std::optional<GameVersion> Configuration::getRawVersion(const std::string& filePath) const
{
	if (!commonItems::DoesFileExist(filePath))
	{
		Log(LogLevel::Warning) << "Failure verifying version: " << filePath << " does not exist. Proceeding blind.";
		return std::nullopt;
	}

	std::ifstream versionFile(filePath);
	if (!versionFile.is_open())
	{
		Log(LogLevel::Warning) << "Failure verifying version: " << filePath << " cannot be opened. Proceeding blind.";
		return std::nullopt;
	}

	while (!versionFile.eof())
	{
		std::string line;
		std::getline(versionFile, line);
		if (line.find("rawVersion") == std::string::npos)
			continue;
		auto pos = line.find(':');
		if (pos == std::string::npos)
		{
			Log(LogLevel::Warning) << "Failure extracting version: " << filePath << " has broken rawVersion. Proceeding blind.";
			return std::nullopt;
		}
		line = line.substr(pos + 1, line.length());
		pos = line.find_first_of('\"');
		if (pos == std::string::npos)
		{
			Log(LogLevel::Warning) << "Failure extracting version: " << filePath << " has broken rawVersion. Proceeding blind.";
			return std::nullopt;
		}
		line = line.substr(pos + 1, line.length());
		pos = line.find_first_of('\"');
		if (pos == std::string::npos)
		{
			Log(LogLevel::Warning) << "Failure extracting version: " << filePath << " has broken rawVersion. Proceeding blind.";
			return std::nullopt;
		}
		line = line.substr(0, pos);
		Log(LogLevel::Info) << "\tVersion is: " << line;
		return GameVersion(line);
	}

	Log(LogLevel::Warning) << "Failure verifying version: " << filePath << " doesn't contain rawVersion. Proceeding blind.";
	return std::nullopt;
}

std::optional<GameVersion> Configuration::getReadmeVersion(const std::string& filePath) const
{
	std::ifstream versionFile(filePath);
	if (!versionFile.is_open())
	{
		Log(LogLevel::Warning) << "Failure verifying version: " << filePath << " cannot be opened. Proceeding blind.";
		return std::nullopt;
	}

	std::string line;
	std::getline(versionFile, line);
	if (versionFile.eof())
	{
		Log(LogLevel::Warning) << "Failure verifying version: " << filePath << " is broken. Proceeding blind.";
		return std::nullopt;
	}
	std::getline(versionFile, line);

	auto pos = line.find(" below");
	if (pos == std::string::npos)
	{
		Log(LogLevel::Warning) << "Failure verifying version: " << filePath << " is broken. Proceeding blind.";
		return std::nullopt;
	}

	line = line.substr(0, pos);
	pos = line.find_last_of(' ');
	line = line.substr(pos + 1, line.length());
	Log(LogLevel::Info) << "\tVersion is: " << line;
	return GameVersion(line);
}

void Configuration::verifyVic2Version(const mappers::ConverterVersion& converterVersion) const
{
	GameVersion vic2version;
	if (commonItems::DoesFileExist(Vic2Path + "/changelog_3.04.txt"))
	{
		Log(LogLevel::Info) << "Version is: 3.0.4";
		return;
	}

	std::string readmePath = Vic2Path + "/ReadMe.txt";
	if (!commonItems::DoesFileExist(readmePath))
	{
		readmePath = Vic2Path + "/Readme.txt";
		if (!commonItems::DoesFileExist(readmePath))
		{
			Log(LogLevel::Error) << "Vic2 version could not be determined, proceeding blind!";
			return;
		}
	}

	const auto V2Version = getReadmeVersion(readmePath);
	if (!V2Version)
	{
		Log(LogLevel::Error) << "Vic2 version could not be determined, proceeding blind!";
		return;
	}

	if (converterVersion.getMinTarget() > *V2Version)
	{
		Log(LogLevel::Error) << "Vic2 version is v" << V2Version->toShortString() << ", converter requires minimum v"
									<< converterVersion.getMinTarget().toShortString() << "!";
		throw std::runtime_error("Converter vs Vic2 installation mismatch!");
	}
	if (!converterVersion.getMaxTarget().isLargerishThan(*V2Version))
	{
		Log(LogLevel::Error) << "Vic2 version is v" << V2Version->toShortString() << ", converter requires maximum v"
									<< converterVersion.getMaxTarget().toShortString() << "!";
		throw std::runtime_error("Converter vs Vic2 installation mismatch!");
	}
}

void Configuration::verifyEU4Version(const mappers::ConverterVersion& converterVersion) const
{
	const auto EU4Version = getRawVersion(EU4Path + "/launcher-settings.json");
	if (!EU4Version)
	{
		Log(LogLevel::Error) << "EU4 version could not be determined, proceeding blind!";
		return;
	}

	if (converterVersion.getMinSource() > *EU4Version)
	{
		Log(LogLevel::Error) << "EU4 version is v" << EU4Version->toShortString() << ", converter requires minimum v"
									<< converterVersion.getMinSource().toShortString() << "!";
		throw std::runtime_error("Converter vs EU4 installation mismatch!");
	}
	if (!converterVersion.getMaxSource().isLargerishThan(*EU4Version))
	{
		Log(LogLevel::Error) << "EU4 version is v" << EU4Version->toShortString() << ", converter requires maximum v"
									<< converterVersion.getMaxSource().toShortString() << "!";
		throw std::runtime_error("Converter vs EU4 installation mismatch!");
	}
}

void Configuration::verifyVic2Path(const std::string& path, bool (*DoesFolderExist)(const std::string& path2), bool (*doesFileExist)(const std::string& path3))
{
	if (!DoesFolderExist(path))
		throw std::runtime_error(path + " does not exist!");
	if (!doesFileExist(path + "/v2game.exe") && !DoesFolderExist(path + "/Victoria 2 - Heart Of Darkness.app") && !DoesFolderExist(path + "../../MacOS"))
		throw std::runtime_error(path + " does not contain Victoria 2!");
}

void Configuration::verifyVic2DocumentsPath(const std::string& path, bool (*DoesFolderExist)(const std::string& path2))
{
	if (!DoesFolderExist(path))
		throw std::runtime_error(path + " does not exist!");
}

void Configuration::setOutputName()
{
	if (incomingOutputName.empty())
	{
		outputName = trimPath(EU4SaveGamePath);
	}
	else
	{
		outputName = incomingOutputName;
	}
	outputName = trimExtension(outputName);
	outputName = replaceCharacter(outputName, '-');
	outputName = replaceCharacter(outputName, ' ');
	theConfiguration.setActualName(outputName);

	outputName = commonItems::normalizeUTF8Path(outputName);
	theConfiguration.setOutputName(outputName);
	LOG(LogLevel::Info) << "Using output name " << outputName;
}

ConfigurationFile::ConfigurationFile(const std::string& filename, const mappers::ConverterVersion& converterVersion)
{
	std::ifstream confFile(filename);
	if (!confFile.is_open())
	{
		Log(LogLevel::Error) << "Cound not open configuration file!";
		throw std::runtime_error("Cound not open configuration file!");
	}

	theConfiguration.instantiate(confFile, converterVersion, commonItems::DoesFolderExist, commonItems::DoesFileExist);

	confFile.close();
}
