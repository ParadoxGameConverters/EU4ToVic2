#include "ProvinceMapper.h"
#include "ProvinceMappingsVersion.h"
#include "../../Configuration.h"
#include "../../EU4World/EU4Version.h"
#include "Log.h"
#include <fstream>
#include <stdexcept>
#include "ParserHelpers.h"

mappers::ProvinceMapper::ProvinceMapper()
{
	LOG(LogLevel::Info) << "Parsing province mappings";
	registerKeys();
	parseFile("configurables/province_mappings.txt");
	clearRegisteredKeywords();

	const auto& mappings = getMappingsVersion(mappingVersions, theConfiguration.getEU4Version());
	createMappings(mappings);
}

mappers::ProvinceMapper::ProvinceMapper(std::istream& theStream, const Configuration& testConfiguration)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();

	const auto& mappings = getMappingsVersion(mappingVersions, testConfiguration.getEU4Version());
	createMappings(mappings);
}

void mappers::ProvinceMapper::registerKeys()
{
	registerRegex("[0-9\\.]+", [this](const std::string& versionString, std::istream& theStream)
		{
			ProvinceMappingsVersion version(versionString, theStream);
			mappingVersions.insert(std::make_pair(version.getVersion(), version));
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}

bool mappers::ProvinceMapper::provinceIsInRegion(int province, const std::string& region) const
{
	return colonialRegionsMapper.provinceIsInRegion(province, region);
}

mappers::ProvinceMappingsVersion mappers::ProvinceMapper::getMappingsVersion(const std::map<EU4::Version, ProvinceMappingsVersion>& mappingsVersions, const EU4::Version& newVersion)
{
	for (auto mappingsVersion = mappingsVersions.rbegin(); mappingsVersion != mappingsVersions.rend(); ++mappingsVersion)
	{
		if (newVersion >= mappingsVersion->first)
		{
			LOG(LogLevel::Info) << "\t-> Using version " << mappingsVersion->first << " mappings";
			return mappingsVersion->second;
		}
	}	
	throw std::range_error("Could not find matching province mappings for EU4 version");
}

void mappers::ProvinceMapper::createMappings(const ProvinceMappingsVersion& provinceMappingsVersion)
{
	for (const auto& mapping: provinceMappingsVersion.getMappings())
	{
		// fix deliberate errors where we leave mappings without keys (asian wasteland comes to mind):
		if (mapping.getVic2Provinces().empty()) continue;
		if (mapping.getEU4Provinces().empty()) continue;
		
		for (const auto& vic2Number: mapping.getVic2Provinces())
		{
			if (vic2Number) vic2ToEU4ProvinceMap.insert(std::make_pair(vic2Number, mapping.getEU4Provinces()));
		}
		for (const auto& eu4Number: mapping.getEU4Provinces())
		{
			if (eu4Number) eu4ToVic2ProvinceMap.insert(std::make_pair(eu4Number, mapping.getVic2Provinces()));
		}
		for (const auto& resettableRegion: mapping.getResettableRegions())
		{
			addProvincesToResettableRegion(resettableRegion, mapping.getVic2Provinces());
		}
	}
}

void mappers::ProvinceMapper::addProvincesToResettableRegion(
	const std::string& regionName,
	const std::vector<int>& provinces
) {
	auto region = resettableProvinces.find(regionName);
	if (region == resettableProvinces.end())
	{
		std::set<int> provincesSet;
		for (const auto& province : provinces)
		{
			provincesSet.insert(province);
		}
		resettableProvinces.insert(std::make_pair(regionName, provincesSet));
	}
	else
	{
		for (const auto& province: provinces)
		{
			region->second.insert(province);
		}
	}
}

std::vector<int> mappers::ProvinceMapper::getVic2ProvinceNumbers(const int eu4ProvinceNumber) const
{
	const auto& mapping = eu4ToVic2ProvinceMap.find(eu4ProvinceNumber);
	if (mapping != eu4ToVic2ProvinceMap.end()) return mapping->second;
	return std::vector<int>();
}

std::vector<int> mappers::ProvinceMapper::getEU4ProvinceNumbers(int vic2ProvinceNumber) const
{
	const auto& mapping = vic2ToEU4ProvinceMap.find(vic2ProvinceNumber);
	if (mapping != vic2ToEU4ProvinceMap.end()) return mapping->second;
	return std::vector<int>();
}

bool mappers::ProvinceMapper::isProvinceResettable(int vic2ProvinceNumber, const std::string& region) const
{
	const auto& regionProvinces = resettableProvinces.find(region);
	if (regionProvinces == resettableProvinces.end()) return false;
	return regionProvinces->second.count(vic2ProvinceNumber) > 0;
}

void mappers::ProvinceMapper::determineValidProvinces()
{
	std::ifstream definitionFile((theConfiguration.getEU4Path() + "/map/definition.csv"));
	if (!definitionFile.is_open()) throw std::runtime_error("Could not open <eu4>/map/definition.csv");

	char input[256];
	while (!definitionFile.eof())
	{
		definitionFile.getline(input, 255);
		std::string inputStr(input);
		if (inputStr.substr(0, 8) == "province" ||
			inputStr.substr(inputStr.find_last_of(';') + 1, 6) == "Unused" ||
			inputStr.substr(inputStr.find_last_of(';') + 1, 3) == "RNW")
		{
			continue;
		}
		auto provNum = std::stoi(inputStr.substr(0, inputStr.find_first_of(';')));
		validProvinces.insert(provNum);
	}
}