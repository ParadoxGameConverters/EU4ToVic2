#include "Configuration.h"
#include "ParserHelpers.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <vector>

Configuration theConfiguration;

void Configuration::instantiate(std::istream& theStream, bool (*doesFolderExist)(const std::string& path2), bool (*doesFileExist)(const std::string& path3))
{
	registerKeyword(std::regex("EU4SaveGame"), [this, doesFolderExist, doesFileExist](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleString path(theStream);
			EU4SaveGamePath = path.getString();
			setOutputName();
		});
	registerKeyword(std::regex("EU4directory"), [this, doesFolderExist, doesFileExist](const std::string& unused, std::istream& theStream){
		const commonItems::singleString path(theStream);
		EU4Path = path.getString();
		verifyEU4Path(EU4Path, doesFolderExist, doesFileExist);
	});
	registerKeyword(std::regex("EU4DocumentsDirectory"), [this](const std::string& unused, std::istream& theStream){
		const commonItems::singleString path(theStream);
		EU4DocumentsPath = path.getString();
	});
	registerKeyword(std::regex("SteamWorkshopDirectory"), [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString path(theStream);
		SteamWorkshopPath = path.getString();
	});
	registerKeyword(std::regex("CK2ExportDirectory"), [this](const std::string& unused, std::istream& theStream){
		const commonItems::singleString path(theStream);
		CK2ExportPath = path.getString();
	});
	registerKeyword(std::regex("Vic2directory"), [this, doesFolderExist, doesFileExist](const std::string& unused, std::istream& theStream){
		const commonItems::singleString path(theStream);
		Vic2Path = path.getString();
		verifyVic2Path(Vic2Path, doesFolderExist, doesFileExist);
	});
	registerKeyword(std::regex("Vic2Documentsdirectory"), [this, doesFolderExist](const std::string& unused, std::istream& theStream){
		const commonItems::singleString path(theStream);
		Vic2DocumentsPath = path.getString();
		verifyVic2DocumentsPath(Vic2DocumentsPath, doesFolderExist);
	});
	registerKeyword(std::regex("Vic2gametype"), commonItems::ignoreItem);
	registerKeyword(std::regex("resetProvinces"), [this](const std::string& unused, std::istream& theStream){
		commonItems::singleString resetProvincesString(theStream);
	});
	registerKeyword(std::regex("max_literacy"), [this](const std::string& unused, std::istream& theStream){
		const commonItems::singleDouble maxLiteracyDouble(theStream);
		MaxLiteracy = maxLiteracyDouble.getDouble();
		LOG(LogLevel::Info) << "Max Literacy: " << MaxLiteracy;
	});
	registerKeyword(std::regex("removeType"), [this](const std::string& unused, std::istream& theStream){
		const commonItems::singleInt removeTypeString(theStream);
		removeType = DEADCORES(removeTypeString.getInt());
	});
	registerKeyword(std::regex("libertyThreshold"), [this](const std::string& unused, std::istream& theStream){
		const commonItems::singleDouble libertyThresholdDouble(theStream);
		libertyThreshold = libertyThresholdDouble.getDouble();
		LOG(LogLevel::Info) << "Liberty Treshold: " << libertyThreshold;
	});
	registerKeyword(std::regex("popShaping"), [this](const std::string& unused, std::istream& theStream){
		const commonItems::singleInt popShapingInt(theStream);
		popShaping = POPSHAPES(popShapingInt.getInt());
		LOG(LogLevel::Info) << "Pop Shaping: " << popShapingInt.getInt();
		});
	registerKeyword(std::regex("coreHandling"), [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt coreHandlingInt(theStream);
		coreHandling = COREHANDLES(coreHandlingInt.getInt());
		LOG(LogLevel::Info) << "Core Handling: " << coreHandlingInt.getInt();
		});
	registerKeyword(std::regex("popShapingFactor"), [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleDouble popShapingFactorDouble(theStream);
		popShapingFactor = popShapingFactorDouble.getDouble();
		LOG(LogLevel::Info) << "Pop Shaping Factor: " << popShapingFactor;
	});
	registerKeyword(std::regex("euroCentrism"), [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt euroCentrismInt(theStream);
		euroCentric = EUROCENTRISM(euroCentrismInt.getInt());
		LOG(LogLevel::Info) << "Eurocentrism: " << euroCentrismInt.getInt();
	});
	registerKeyword(std::regex("debug"), [this](const std::string& unused, std::istream& theStream){
		const commonItems::singleString debugString(theStream);
		debug = (debugString.getString() == "yes");
	});
	registerKeyword(std::regex("randomiseRgos"), [this](const std::string& unused, std::istream& theStream){
		const commonItems::singleString randomiseRgosString(theStream);
		randomiseRgos = (randomiseRgosString.getString() == "yes");
		LOG(LogLevel::Info) << "Randomise RGOs: " << randomiseRgosString.getString();
	});

	LOG(LogLevel::Info) << "Reading configuration file";
	parseStream(theStream);
}


void Configuration::verifyEU4Path(const std::string& path, bool (*doesFolderExist)(const std::string& path2), bool (*doesFileExist)(const std::string& path3))
{
	if (!doesFolderExist(path)) throw std::runtime_error(path + " does not exist!");
	if (!doesFileExist(path + "/eu4.exe")) throw std::runtime_error(path + " does not contain Europa Universalis 4!");
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
	outputName = trimPath(EU4SaveGamePath);
	outputName = trimExtension(outputName);
	outputName = replaceCharacter(outputName, '-');
	outputName = replaceCharacter(outputName, ' ');

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
	registerKeyword(std::regex("configuration"), [](const std::string& unused, std::istream& theStream){
		theConfiguration.instantiate(theStream, Utils::doesFolderExist, Utils::DoesFileExist);
	});

	parseFile(filename);
}
