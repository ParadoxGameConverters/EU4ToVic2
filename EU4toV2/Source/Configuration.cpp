#include "Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include <fstream>
#include "CommonFunctions.h"

Configuration theConfiguration;

void Configuration::instantiate(std::istream& theStream, bool (*DoesFolderExist)(const std::string& path2), bool (*doesFileExist)(const std::string& path3))
{
	registerKeyword("SaveGame", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString path(theStream);
		EU4SaveGamePath = path.getString();
		LOG(LogLevel::Info) << "EU4 savegame path: " << EU4SaveGamePath;
	});
	registerKeyword("EU4directory", [this, DoesFolderExist, doesFileExist](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString path(theStream);
		EU4Path = path.getString();
		verifyEU4Path(EU4Path, DoesFolderExist, doesFileExist);
		LOG(LogLevel::Info) << "EU4 path: " << EU4Path;
	});
	registerKeyword("EU4DocumentsDirectory", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString path(theStream);
		EU4DocumentsPath = path.getString();
		LOG(LogLevel::Info) << "EU4 documents path: " << EU4DocumentsPath;
	});
	registerKeyword("SteamWorkshopDirectory", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString path(theStream);
		SteamWorkshopPath = path.getString();
		LOG(LogLevel::Info) << "EU4 steam workshop path: " << SteamWorkshopPath;
	});
	registerKeyword("CK2ExportDirectory", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString path(theStream);
		CK2ExportPath = path.getString();
		LOG(LogLevel::Info) << "CK2 Export path: " << CK2ExportPath;
	});
	registerKeyword("Vic2directory", [this, DoesFolderExist, doesFileExist](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString path(theStream);
		Vic2Path = path.getString();
		verifyVic2Path(Vic2Path, DoesFolderExist, doesFileExist);
		LOG(LogLevel::Info) << "Vic2 path: " << Vic2Path;
	});
	registerKeyword("Vic2Documentsdirectory", [this, DoesFolderExist](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString path(theStream);
		Vic2DocumentsPath = path.getString();
		verifyVic2DocumentsPath(Vic2DocumentsPath, DoesFolderExist);
		LOG(LogLevel::Info) << "Vic2 documents path: " << Vic2DocumentsPath;
	});
	registerKeyword("max_literacy", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString maxLiteracyString(theStream);
		MaxLiteracy = static_cast<double>(std::stoi(maxLiteracyString.getString())) / 100;
		LOG(LogLevel::Info) << "Max Literacy: " << MaxLiteracy;
	});
	registerKeyword("remove_type", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString removeTypeString(theStream);
		removeType = DEADCORES(std::stoi(removeTypeString.getString()));
	});
	registerKeyword("absorb_colonies", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString absorbString(theStream);
		absorbColonies = ABSORBCOLONIES(std::stoi(absorbString.getString()));
		LOG(LogLevel::Info) << "Absorb Colonies: " << absorbString.getString();
	});
	registerKeyword("liberty_threshold", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString libertyThresholdString(theStream);
		libertyThreshold = LIBERTYDESIRE(std::stoi(libertyThresholdString.getString()));
		LOG(LogLevel::Info) << "Liberty Treshold: " << libertyThresholdString.getString();
	});
	registerKeyword("pop_shaping", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString popShapingString(theStream);
		popShaping = POPSHAPES(std::stoi(popShapingString.getString()));
		LOG(LogLevel::Info) << "Pop Shaping: " << popShapingString.getString();
	});
	registerKeyword("core_handling", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString coreHandlingString(theStream);
		coreHandling = COREHANDLES(std::stoi(coreHandlingString.getString()));
		LOG(LogLevel::Info) << "Core Handling: " << coreHandlingString.getString();
	});
	registerKeyword("pop_shaping_factor", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString popShapingFactorString(theStream);
		popShapingFactor = static_cast<double>(std::stoi(popShapingFactorString.getString()));
		LOG(LogLevel::Info) << "Pop Shaping Factor: " << popShapingFactor;
	});
	registerKeyword("euro_centrism", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString euroCentrismString(theStream);
		euroCentric = EUROCENTRISM(std::stoi(euroCentrismString.getString()));
		LOG(LogLevel::Info) << "Eurocentrism: " << euroCentrismString.getString();
	});
	registerKeyword("africa_reset", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString africaResetString(theStream);
		africaReset = AFRICARESET(std::stoi(africaResetString.getString()));
		LOG(LogLevel::Info) << "Africa Reset: " << africaResetString.getString();
	});
	registerKeyword("debug", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString debugString(theStream);
		debug = (debugString.getString() == "yes");
	});
	registerKeyword("randomise_rgos", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString randomiseRgosString(theStream);
		randomiseRgos = randomiseRgosString.getString() == "yes";
		LOG(LogLevel::Info) << "Randomise RGOs: " << randomiseRgosString.getString();
	});
	registerKeyword("convert_all", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString convertAllString(theStream);
		convertAll = convertAllString.getString() == "yes";
		LOG(LogLevel::Info) << "Convert All: " << convertAllString.getString();
	});
	registerKeyword("output_name", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString outputNameStr(theStream);
		incomingOutputName = outputNameStr.getString();
		LOG(LogLevel::Info) << "Output Name: " << incomingOutputName;
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	LOG(LogLevel::Info) << "Reading configuration file";
	parseStream(theStream);
	clearRegisteredKeywords();
	setOutputName();
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

ConfigurationFile::ConfigurationFile(const std::string& filename)
{
	std::ifstream confFile(filename);
	if (!confFile.is_open())
	{
		Log(LogLevel::Error) << "Cound not open configuration file!";
		throw std::runtime_error("Cound not open configuration file!");
	}

	theConfiguration.instantiate(confFile, commonItems::DoesFolderExist, commonItems::DoesFileExist);

	confFile.close();
}
