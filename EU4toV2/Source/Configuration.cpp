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



#include "Configuration.h"
#include "ParserHelpers.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <vector>



Configuration theConfiguration;



void Configuration::instantiate(std::istream& theStream, bool (*doesFolderExist)(const std::string& path), bool (*doesFileExist)(const std::string& path))
{
	registerKeyword(std::regex("EU4directory"), [this, doesFolderExist, doesFileExist](const std::string& unused, std::istream& theStream){
		commonItems::singleString path(theStream);
		EU4Path = path.getString();
		verifyEU4Path(EU4Path, doesFolderExist, doesFileExist);
	});
	registerKeyword(std::regex("EU4DocumentsDirectory"), [this](const std::string& unused, std::istream& theStream){
		commonItems::singleString path(theStream);
		EU4DocumentsPath = path.getString();
	});
	registerKeyword(std::regex("SteamWorkshopDirectory"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleString path(theStream);
		SteamWorkshopPath = path.getString();
	});
	registerKeyword(std::regex("CK2ExportDirectory"), [this](const std::string& unused, std::istream& theStream){
		commonItems::singleString path(theStream);
		CK2ExportPath = path.getString();
	});
	registerKeyword(std::regex("Vic2directory"), [this, doesFolderExist, doesFileExist](const std::string& unused, std::istream& theStream){
		commonItems::singleString path(theStream);
		Vic2Path = path.getString();
		verifyVic2Path(Vic2Path, doesFolderExist, doesFileExist);
	});
	registerKeyword(std::regex("Vic2Documentsdirectory"), [this, doesFolderExist](const std::string& unused, std::istream& theStream){
		commonItems::singleString path(theStream);
		Vic2DocumentsPath = path.getString();
		verifyVic2DocumentsPath(Vic2DocumentsPath, doesFolderExist);
	});
	registerKeyword(std::regex("Vic2gametype"), [this](const std::string& unused, std::istream& theStream){
		commonItems::singleString gameTypeString(theStream);
		Vic2Gametype = gameTypeString.getString();
	});
	registerKeyword(std::regex("resetProvinces"), [this](const std::string& unused, std::istream& theStream){
		commonItems::singleString resetProvincesString(theStream);
	});
	registerKeyword(std::regex("max_literacy"), [this](const std::string& unused, std::istream& theStream){
		commonItems::singleDouble maxLiteracyDouble(theStream);
		MaxLiteracy = maxLiteracyDouble.getDouble();
	});
	registerKeyword(std::regex("Removetype"), [this](const std::string& unused, std::istream& theStream){
		commonItems::singleInt removeTypeString(theStream);
		removeType = Configuration::DEADCORES(removeTypeString.getInt());
	});
	registerKeyword(std::regex("libertyThreshold"), [this](const std::string& unused, std::istream& theStream){
		commonItems::singleDouble libertyThresholdDouble(theStream);
		libertyThreshold = libertyThresholdDouble.getDouble();
	});
	registerKeyword(std::regex("popShaping"), [this](const std::string& unused, std::istream& theStream){
		commonItems::singleInt popShapingString(theStream);
		popShaping = Configuration::POPSHAPES(popShapingString.getInt());
	});
	registerKeyword(std::regex("coreHandling"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt coreHandlingString(theStream);
		coreHandling = Configuration::COREHANDLES(coreHandlingString.getInt());
	});
	registerKeyword(std::regex("popShapingFactor"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleDouble popShapingFactorDouble(theStream);
		popShapingFactor = popShapingFactorDouble.getDouble();
		LOG(LogLevel::Info) << "Pop Shaping Factor: " << popShapingFactor;
	});
	registerKeyword(std::regex("debug"), [this](const std::string& unused, std::istream& theStream){
		commonItems::singleString debugString(theStream);
		debug = (debugString.getString() == "yes");
	});

	LOG(LogLevel::Info) << "Reading configuration file";
	parseStream(theStream);
}


void Configuration::verifyEU4Path(const std::string& path, bool (*doesFolderExist)(const std::string& path), bool (*doesFileExist)(const std::string& path))
{
	if (!doesFolderExist(path))
	{
		LOG(LogLevel::Error) << path << " does not exist";
		exit(-1);
	}
	else if (!doesFileExist(path + "/eu4.exe"))
	{
		LOG(LogLevel::Error) << path << " does not contain Europa Universalis 4";
		exit(-1);
	}

	else if (!doesFileExist(path + "/map/positions.txt"))
	{
		LOG(LogLevel::Error) << path << " does not appear to be a valid EU4 install";
		exit(-1);
	}
	else
	{
		LOG(LogLevel::Debug) << "EU4 install path is " << path;
	}
}


void Configuration::verifyVic2Path(const std::string& path, bool (*doesFolderExist)(const std::string& path), bool (*doesFileExist)(const std::string& path))
{
	if (!doesFolderExist(path))
	{
		LOG(LogLevel::Error) << path << " does not exist";
		exit(-1);
	}
	else if (!doesFileExist(path + "/v2game.exe"))
	{
		LOG(LogLevel::Error) << path << " does not contain Victoria 2";
		exit(-1);
	}
	else
	{
		LOG(LogLevel::Debug) << "Victoria 2 install path is " << path;
	}
}


void Configuration::verifyVic2DocumentsPath(const std::string& path, bool (*doesFolderExist)(const std::string& path))
{
	if (!doesFolderExist(path))
	{
		LOG(LogLevel::Error) << path << " does not exist";
		exit(-1);
	}
	else
	{
		LOG(LogLevel::Debug) << "Victoria 2 documents directory is " << path;
	}
}


bool Configuration::wasDLCActive(const std::string& DLC)
{
	for (auto activeDLC: activeDLCs)
	{
		if (DLC == activeDLC)
		{
			return true;
		}
	}
	return false;
}


ConfigurationFile::ConfigurationFile(const std::string& filename)
{
	registerKeyword(std::regex("configuration"), [](const std::string& unused, std::istream& theStream){
		theConfiguration.instantiate(theStream, Utils::doesFolderExist, Utils::DoesFileExist);
	});

	parseFile(filename);
}