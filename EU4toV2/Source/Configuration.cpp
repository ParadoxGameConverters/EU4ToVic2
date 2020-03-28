#include "Configuration.h"
#include "ParserHelpers.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <vector>

Configuration theConfiguration;

void Configuration::instantiate(std::istream& theStream, bool (*doesFolderExist)(const std::string& path2), bool (*doesFileExist)(const std::string& path3))
{
	registerKeyword("SaveGame", [this](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleString path(theStream);
			EU4SaveGamePath = path.getString();
		});
	registerKeyword("EU4directory", [this, doesFolderExist, doesFileExist](const std::string& unused, std::istream& theStream){
		const commonItems::singleString path(theStream);
		EU4Path = path.getString();
		verifyEU4Path(EU4Path, doesFolderExist, doesFileExist);
	});
	registerKeyword("EU4DocumentsDirectory", [this](const std::string& unused, std::istream& theStream){
		const commonItems::singleString path(theStream);
		EU4DocumentsPath = path.getString();
	});
	registerKeyword("SteamWorkshopDirectory", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString path(theStream);
		SteamWorkshopPath = path.getString();
	});
	registerKeyword("CK2ExportDirectory", [this](const std::string& unused, std::istream& theStream){
		const commonItems::singleString path(theStream);
		CK2ExportPath = path.getString();
	});
	registerKeyword("Vic2directory", [this, doesFolderExist, doesFileExist](const std::string& unused, std::istream& theStream){
		const commonItems::singleString path(theStream);
		Vic2Path = path.getString();
		verifyVic2Path(Vic2Path, doesFolderExist, doesFileExist);
	});
	registerKeyword("Vic2Documentsdirectory", [this, doesFolderExist](const std::string& unused, std::istream& theStream){
		const commonItems::singleString path(theStream);
		Vic2DocumentsPath = path.getString();
		verifyVic2DocumentsPath(Vic2DocumentsPath, doesFolderExist);
	});
	registerKeyword("reset_provinces", commonItems::ignoreItem);
	registerKeyword("max_literacy", [this](const std::string& unused, std::istream& theStream){
		const commonItems::singleDouble maxLiteracyDouble(theStream);
		MaxLiteracy = maxLiteracyDouble.getDouble() / 100;
		LOG(LogLevel::Info) << "Max Literacy: " << MaxLiteracy;
	});
	registerKeyword("remove_type", [this](const std::string& unused, std::istream& theStream){
		const commonItems::singleInt removeTypeString(theStream);
		removeType = DEADCORES(removeTypeString.getInt());
	});
	registerKeyword("absorb_colonies", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt absorbInt(theStream);
		absorbColonies = ABSORBCOLONIES(absorbInt.getInt());
		LOG(LogLevel::Info) << "Absorb Colonies: " << absorbInt.getInt();
		});
	registerKeyword("liberty_threshold", [this](const std::string& unused, std::istream& theStream){
		const commonItems::singleInt libertyThresholdInt(theStream);
		libertyThreshold = LIBERTYDESIRE(libertyThresholdInt.getInt());
		LOG(LogLevel::Info) << "Liberty Treshold: " << libertyThresholdInt.getInt();
	});
	registerKeyword("pop_shaping", [this](const std::string& unused, std::istream& theStream){
		const commonItems::singleInt popShapingInt(theStream);
		popShaping = POPSHAPES(popShapingInt.getInt());
		LOG(LogLevel::Info) << "Pop Shaping: " << popShapingInt.getInt();
		});
	registerKeyword("core_handling", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt coreHandlingInt(theStream);
		coreHandling = COREHANDLES(coreHandlingInt.getInt());
		LOG(LogLevel::Info) << "Core Handling: " << coreHandlingInt.getInt();
		});
	registerKeyword("pop_shaping_factor", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleDouble popShapingFactorDouble(theStream);
		popShapingFactor = popShapingFactorDouble.getDouble();
		LOG(LogLevel::Info) << "Pop Shaping Factor: " << popShapingFactor;
	});
	registerKeyword("euro_centrism", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt euroCentrismInt(theStream);
		euroCentric = EUROCENTRISM(euroCentrismInt.getInt());
		LOG(LogLevel::Info) << "Eurocentrism: " << euroCentrismInt.getInt();
		});
	registerKeyword("africa_reset", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt africaResetInt(theStream);
		africaReset = AFRICARESET(africaResetInt.getInt());
		LOG(LogLevel::Info) << "Africa Reset: " << africaResetInt.getInt();
		});
	registerKeyword("debug", [this](const std::string& unused, std::istream& theStream){
		const commonItems::singleString debugString(theStream);
		debug = (debugString.getString() == "yes");
	});
	registerKeyword("randomise_rgos", [this](const std::string& unused, std::istream& theStream){
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
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	LOG(LogLevel::Info) << "Reading configuration file";
	parseStream(theStream);
	clearRegisteredKeywords();
	setOutputName();
}


void Configuration::verifyEU4Path(const std::string& path, bool (*doesFolderExist)(const std::string& path2), bool (*doesFileExist)(const std::string& path3))
{
	if (!doesFolderExist(path)) throw std::runtime_error(path + " does not exist!");
	if (!doesFileExist(path + "/eu4.exe") && !doesFileExist(path + "/eu4")) throw std::runtime_error(path + " does not contain Europa Universalis 4!");
	if (!doesFileExist(path + "/map/positions.txt")) throw std::runtime_error(path + " does not appear to be a valid EU4 install!");
	LOG(LogLevel::Info) << "\tEU4 install path is " << path;
}


void Configuration::verifyVic2Path(const std::string& path, bool (*doesFolderExist)(const std::string& path2), bool (*doesFileExist)(const std::string& path3))
{
	if (!doesFolderExist(path)) throw std::runtime_error(path + " does not exist!");
	if (!doesFileExist(path + "/v2game.exe")) throw std::runtime_error(path + " does not contain Victoria 2!");
	LOG(LogLevel::Info) << "\tVictoria 2 install path is " << path;
}


void Configuration::verifyVic2DocumentsPath(const std::string& path, bool (*doesFolderExist)(const std::string& path2))
{
	if (!doesFolderExist(path)) throw std::runtime_error(path + " does not exist!");
	LOG(LogLevel::Info) << "\tVictoria 2 documents directory is " << path;
}


bool Configuration::wasDLCActive(const std::string& DLC) const
{
	for (const auto& activeDLC: activeDLCs) if (DLC == activeDLC) return true;

	return false;
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
	
	outputName = Utils::normalizeUTF8Path(outputName);
	theConfiguration.setOutputName(outputName);
	LOG(LogLevel::Info) << "Using output name " << outputName;
}

std::string Configuration::trimPath(const std::string& fileName)
{
	const int lastSlash = fileName.find_last_of("\\");
	return fileName.substr(lastSlash + 1, fileName.length());
}

std::string Configuration::trimExtension(const std::string& fileName)
{
	const int length = fileName.find_last_of(".");
	return fileName.substr(0, length);
}

std::string Configuration::replaceCharacter(std::string fileName, char character)
{
	auto position = fileName.find_first_of(character);
	while (position != std::string::npos)
	{
		fileName.replace(position, 1, "_");
		position = fileName.find_first_of(character);
	}

	return fileName;
}

ConfigurationFile::ConfigurationFile(const std::string& filename)
{
	registerKeyword("configuration", [](const std::string& unused, std::istream& theStream){
		theConfiguration.instantiate(theStream, Utils::doesFolderExist, Utils::DoesFileExist);
	});

	parseFile(filename);
}
