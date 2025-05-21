#include "Configuration.h"
#include "CommonFunctions.h"
#include "CommonRegexes.h"
#include "GameVersion.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include <fstream>
Configuration theConfiguration;

void Configuration::instantiate(std::istream& theStream,
	 const commonItems::ConverterVersion& converterVersion,
	 bool (*DoesFolderExist)(const std::filesystem::path& path2),
	 bool (*doesFileExist)(const std::filesystem::path& path3))
{
	registerKeyword("SaveGame", [this](std::istream& theStream) {
		EU4SaveGamePath = std::filesystem::path(commonItems::getString(theStream));
		Log(LogLevel::Info) << "EU4 savegame path: " << EU4SaveGamePath.string();
	});
	registerKeyword("EU4directory", [this, converterVersion, DoesFolderExist, doesFileExist](std::istream& theStream) {
		EU4Path = std::filesystem::path(commonItems::getString(theStream));
		verifyEU4Path(EU4Path, DoesFolderExist, doesFileExist);
		Log(LogLevel::Info) << "EU4 path: " << EU4Path.string();
		verifyEU4Version(converterVersion);
	});
	registerKeyword("EU4DocumentsDirectory", [this](std::istream& theStream) {
		EU4DocumentsPath = std::filesystem::path(commonItems::getString(theStream));
		Log(LogLevel::Info) << "EU4 documents path: " << EU4DocumentsPath.string();
	});
	registerKeyword("Vic2directory", [this, converterVersion, DoesFolderExist, doesFileExist](std::istream& theStream) {
		Vic2Path = std::filesystem::path(commonItems::getString(theStream));
		verifyVic2Path(Vic2Path, DoesFolderExist, doesFileExist);
		Log(LogLevel::Info) << "Vic2 path: " << Vic2Path.string();
		verifyVic2Version(converterVersion);
	});
	registerKeyword("max_literacy", [this](std::istream& theStream) {
		const auto maxLiteracyString = commonItems::getString(theStream);
		MaxLiteracy = static_cast<double>(std::stoi(maxLiteracyString)) / 100;
		Log(LogLevel::Info) << "Max Literacy: " << MaxLiteracy;
	});
	registerKeyword("remove_type", [this](std::istream& theStream) {
		const auto removeTypeString = commonItems::getString(theStream);
		removeType = DEADCORES(std::stoi(removeTypeString));
	});
	registerKeyword("absorb_colonies", [this](std::istream& theStream) {
		const auto absorbString = commonItems::getString(theStream);
		absorbColonies = ABSORBCOLONIES(std::stoi(absorbString));
		Log(LogLevel::Info) << "Absorb Colonies: " << absorbString;
	});
	registerKeyword("liberty_threshold", [this](std::istream& theStream) {
		const auto libertyThresholdString = commonItems::getString(theStream);
		libertyThreshold = LIBERTYDESIRE(std::stoi(libertyThresholdString));
		Log(LogLevel::Info) << "Liberty Treshold: " << libertyThresholdString;
	});
	registerKeyword("pop_shaping", [this](std::istream& theStream) {
		const auto popShapingString = commonItems::getString(theStream);
		popShaping = POPSHAPES(std::stoi(popShapingString));
		Log(LogLevel::Info) << "Pop Shaping: " << popShapingString;
	});
	registerKeyword("core_handling", [this](std::istream& theStream) {
		const auto coreHandlingString = commonItems::getString(theStream);
		coreHandling = COREHANDLES(std::stoi(coreHandlingString));
		Log(LogLevel::Info) << "Core Handling: " << coreHandlingString;
	});
	registerKeyword("pop_shaping_factor", [this](std::istream& theStream) {
		const auto popShapingFactorString = commonItems::getString(theStream);
		popShapingFactor = static_cast<double>(std::stoi(popShapingFactorString));
		Log(LogLevel::Info) << "Pop Shaping Factor: " << popShapingFactor;
	});
	registerKeyword("euro_centrism", [this](std::istream& theStream) {
		const auto euroCentrismString = commonItems::getString(theStream);
		euroCentric = EUROCENTRISM(std::stoi(euroCentrismString));
		Log(LogLevel::Info) << "Eurocentrism: " << euroCentrismString;
	});
	registerKeyword("africa_reset", [this](std::istream& theStream) {
		const auto africaResetString = commonItems::getString(theStream);
		africaReset = AFRICARESET(std::stoi(africaResetString));
		Log(LogLevel::Info) << "Africa Reset: " << africaResetString;
	});
	registerKeyword("debug", [this](std::istream& theStream) {
		const auto debugString = commonItems::getString(theStream);
		debug = (debugString == "yes");
	});
	registerKeyword("randomise_rgos", [this](std::istream& theStream) {
		const auto randomiseRgosString = commonItems::getString(theStream);
		randomiseRgos = randomiseRgosString == "yes";
		Log(LogLevel::Info) << "Randomise RGOs: " << randomiseRgosString;
	});
	registerKeyword("convert_all", [this](std::istream& theStream) {
		const auto convertAllString = commonItems::getString(theStream);
		convertAll = convertAllString == "yes";
		Log(LogLevel::Info) << "Convert All: " << convertAllString;
	});
	registerKeyword("hybrid_mod", [this](std::istream& theStream) {
		const auto hybridModString = commonItems::getString(theStream);
		hybridMod = HYBRIDMOD(std::stoi(hybridModString));
		Log(LogLevel::Info) << "Hybrid mod: " << hybridModString;
	});
	registerKeyword("output_name", [this](std::istream& theStream) {
		incomingOutputName = std::filesystem::path(commonItems::getString(theStream));
		Log(LogLevel::Info) << "Output Name: " << incomingOutputName.string();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	Log(LogLevel::Info) << "Reading configuration file";
	parseStream(theStream);
	clearRegisteredKeywords();
	setOutputName();
	verifyHPMInstallPath(DoesFolderExist);
	Log(LogLevel::Progress) << "3 %";
}

void Configuration::verifyHPMInstallPath(bool (*DoesFolderExist)(const std::filesystem::path& path2))
{
	if (DoesFolderExist(Vic2Path / "mod/HPM"))
		HPMverified = true;

	if (isHpmEnabled())
	{
		if (HPMverified)
			swapInstallationPathToHPM();
		else
			throw std::runtime_error("HPM installation cannot be found in " + Vic2Path.string() + "/mod/HPM");
	}
}

void Configuration::swapInstallationPathToHPM()
{
	VanillaVic2Path = Vic2Path; // necessary for importing province localisations
	Vic2Path = Vic2Path / "mod/HPM";
}

void Configuration::verifyEU4Path(const std::filesystem::path& path,
	 bool (*DoesFolderExist)(const std::filesystem::path& path2),
	 bool (*doesFileExist)(const std::filesystem::path& path3))
{
	if (!DoesFolderExist(path))
		throw std::runtime_error(path.string() + " does not exist!");
	if (!doesFileExist(path / "eu4.exe") && !doesFileExist(path / "eu4") && !DoesFolderExist(path / "eu4.app"))
		throw std::runtime_error(path.string() + " does not contain Europa Universalis 4!");
	if (!doesFileExist(path / "map/positions.txt"))
		throw std::runtime_error(path.string() + " does not appear to be a valid EU4 install!");
}

void Configuration::verifyVic2Version(const commonItems::ConverterVersion& converterVersion) const
{
	GameVersion vic2version;
	if (commonItems::DoesFileExist(Vic2Path / "changelog_3.04.txt"))
	{
		Log(LogLevel::Info) << "Vic2 version: 3.0.4";
		return;
	}

	std::filesystem::path readmePath = Vic2Path / "ReadMe.txt";
	if (!commonItems::DoesFileExist(readmePath))
	{
		readmePath = Vic2Path / "Readme.txt";
		if (!commonItems::DoesFileExist(readmePath))
		{
			Log(LogLevel::Error) << "Vic2 version could not be determined, proceeding blind!";
			return;
		}
	}

	const auto V2Version = GameVersion::extractVersionFromReadMe(readmePath);
	if (!V2Version)
	{
		Log(LogLevel::Error) << "Vic2 version could not be determined, proceeding blind!";
		return;
	}

	Log(LogLevel::Info) << "Vic2 version: " << V2Version->toShortString();

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

void Configuration::verifyEU4Version(const commonItems::ConverterVersion& converterVersion) const
{
	const auto EU4Version = GameVersion::extractVersionFromLauncher(EU4Path / "launcher-settings.json");
	if (!EU4Version)
	{
		Log(LogLevel::Error) << "EU4 version could not be determined, proceeding blind!";
		return;
	}

	Log(LogLevel::Info) << "EU4 version: " << EU4Version->toShortString();

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

void Configuration::verifyVic2Path(const std::filesystem::path& path,
	 bool (*DoesFolderExist)(const std::filesystem::path& path2),
	 bool (*doesFileExist)(const std::filesystem::path& path3))
{
	if (!DoesFolderExist(path))
		throw std::runtime_error(path.string() + " does not exist!");
	if (!doesFileExist(path / "v2game.exe") && !DoesFolderExist(path / "Victoria 2 - Heart Of Darkness.app") && !DoesFolderExist(path / "../../MacOS"))
		throw std::runtime_error(path.string() + " does not contain Victoria 2!");
}

void Configuration::verifyVic2DocumentsPath(const std::filesystem::path& path, bool (*DoesFolderExist)(const std::filesystem::path& path2))
{
	if (!DoesFolderExist(path))
		throw std::runtime_error(path.string() + " does not exist!");
}

void Configuration::setOutputName()
{
	if (incomingOutputName.empty())
	{
		outputName = EU4SaveGamePath.stem();
	}
	else
	{
		outputName = incomingOutputName;
	}
	outputName = outputName.stem();
	outputName = std::filesystem::path(replaceCharacter(outputName.string(), '-'));
	outputName = std::filesystem::path(replaceCharacter(outputName.string(), ' '));
	theConfiguration.setActualName(outputName);

	outputName = std::filesystem::path(commonItems::normalizeUTF8Path(outputName.string()));
	theConfiguration.setOutputName(outputName);
	Log(LogLevel::Info) << "Using output name " << outputName.string();
}

ConfigurationFile::ConfigurationFile(const std::filesystem::path& filename, const commonItems::ConverterVersion& converterVersion)
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
