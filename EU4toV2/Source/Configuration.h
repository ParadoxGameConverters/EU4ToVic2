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



#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_


#include "EU4World/EU4Version.h"
#include "Date.h"
#include "newParser.h"
#include <string>
#include <vector>



class Configuration: commonItems::parser
{
	public:
		Configuration() = default;
		Configuration(const Configuration&) = default;
		Configuration(Configuration&&) = default;
		Configuration& operator=(const Configuration&) = default;
		Configuration& operator=(Configuration&&) = default;
		~Configuration() = default;

		void instantiate(std::istream& theStream, bool (*doesFolderExist)(const std::string& path), bool (*doesFileExist)(const std::string& path));

		std::string getEU4Path() { return EU4Path; }
		std::string getEU4DocumentsPath() { return EU4DocumentsPath; }
		std::string getCK2ExportPath() { return CK2ExportPath; }
		std::string getVic2Path() { return Vic2Path; }
		std::string getVic2DocumentsPath() { return Vic2DocumentsPath; }
		std::string getVic2Gametype() { return Vic2Gametype; }
		std::string getResetProvinces() { return resetProvinces; }
		double getMaxLiteracy() { return MaxLiteracy; }
		std::string getRemovetype() { return Removetype; }
		double getLibertyThreshold() { return libertyThreshold; }
		bool getConvertPopTotals() { return convertPopTotals; }
		bool getDebug() { return debug; }

		date getFirstEU4Date() { return firstEU4Date; }
		date getLastEU4Date() { return lastEU4Date; }
		std::string getOutputName() { return outputName; }
		std::vector<std::string> getEU4Mods() { return EU4Mods; }

		void setFirstEU4Date(date _firstDate) { firstEU4Date = _firstDate; }
		void setLastEU4Date(date _lastDate) { lastEU4Date = _lastDate; }
		void setOutputName(std::string name) { outputName = name; }
		void addEU4Mod(std::string mod) { EU4Mods.push_back(mod); }
		void setEU4Version(EU4::Version version) { version = version; }
		void setActiveDLCs(std::vector<std::string> _activeDLCs) { activeDLCs = _activeDLCs; }

		bool versionLessThan(const std::string& versionString);
		bool wasDLCActive(const std::string& DLC);

	private:
		void verifyEU4Path(const std::string& path, bool (*doesFolderExist)(const std::string& path), bool (*doesFileExist)(const std::string& path));
		void verifyVic2Path(const std::string& path, bool (*doesFolderExist)(const std::string& path), bool (*doesFileExist)(const std::string& path));
		void verifyVic2DocumentsPath(const std::string& path, bool (*doesFolderExist)(const std::string& path));

		// options from configuration.txt
		std::string EU4Path;
		std::string EU4DocumentsPath;
		std::string CK2ExportPath;
		std::string Vic2Path;
		std::string Vic2DocumentsPath;
		std::string Vic2Gametype = "HOD";
		std::string resetProvinces = "no";
		double MaxLiteracy = 1.0;
		std::string Removetype = "none";
		double libertyThreshold = 100.0;
		bool convertPopTotals = false;
		bool debug = false;
	
		// items set during conversion
		EU4::Version version;
		date firstEU4Date;
		date lastEU4Date;
		std::string outputName;
		std::vector<std::string> activeDLCs;
		std::vector<std::string> EU4Mods;
};


extern Configuration theConfiguration;


class ConfigurationFile: commonItems::parser
{
	public:
		explicit ConfigurationFile(const std::string& filename);
		~ConfigurationFile() = default;

	private:
		ConfigurationFile() = delete;
		ConfigurationFile(const ConfigurationFile&) = delete;
		ConfigurationFile(ConfigurationFile&&) = delete;
		ConfigurationFile& operator=(const ConfigurationFile&) = delete;
		ConfigurationFile& operator=(ConfigurationFile&&) = delete;
};



#endif // CONFIGURATION_H_
