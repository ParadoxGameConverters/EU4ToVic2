#include "ProvinceMapper.h"
#include "ProvinceMappingsVersion.h"
#include "../../Configuration.h"
#include "../../EU4World/EU4Version.h"
#include "Log.h"
#include <fstream>
#include <stdexcept>

mappers::ProvinceMapper::ProvinceMapper(std::istream& theStream, const Configuration& configuration)
{
	std::map<EU4::Version, ProvinceMappingsVersion> mappingVersions;

	registerRegex("[0-9\\.]+", [&mappingVersions](const std::string& versionString, std::istream& theStream)
		{
			ProvinceMappingsVersion version(versionString, theStream);
			mappingVersions.insert(std::make_pair(version.getVersion(), version));
		});

	parseStream(theStream);

	ProvinceMappingsVersion mappings = getMappingsVersion(mappingVersions, configuration);
	createMappings(mappings);
}

bool mappers::ProvinceMapper::provinceIsInRegion(int province, const std::string& region) const
{
	return colonialRegionsMapper.provinceIsInRegion(province, region);
}

mappers::ProvinceMappingsVersion mappers::ProvinceMapper::getMappingsVersion(
	const std::map<EU4::Version, ProvinceMappingsVersion>& mappingsVersions,
	const Configuration& configuration
) {
	EU4::Version saveVersion = configuration.getEU4Version();
	for (auto mappingsVersion = mappingsVersions.rbegin(); mappingsVersion != mappingsVersions.rend(); mappingsVersion++)
	{
		if (saveVersion >= mappingsVersion->first)
		{
			LOG(LogLevel::Debug) << "Using version " << mappingsVersion->first << " mappings";
			return mappingsVersion->second;
		}
	}

	std::range_error exception("Could not find matching province mappings for EU4 version");
	throw exception;
}

void mappers::ProvinceMapper::createMappings(const ProvinceMappingsVersion& provinceMappingsVersion)
{
	for (auto mapping: provinceMappingsVersion.getMappings())
	{
		for (auto Vic2Number: mapping.getVic2Provinces())
		{
			if (Vic2Number != 0)
			{
				Vic2ToEU4ProvinceMap.insert(std::make_pair(Vic2Number, mapping.getEU4Provinces()));
			}
		}
		for (auto EU4Number: mapping.getEU4Provinces())
		{
			if (EU4Number != 0)
			{
				EU4ToVic2ProvinceMap.insert(std::make_pair(EU4Number, mapping.getVic2Provinces()));
			}
		}
		for (auto resettableRegion: mapping.getResettableRegions())
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
		for (auto province : provinces)
		{
			provincesSet.insert(province);
		}
		resettableProvinces.insert(std::make_pair(regionName, provincesSet));
	}
	else
	{
		for (auto province: provinces)
		{
			region->second.insert(province);
		}
	}
}

std::vector<int> mappers::ProvinceMapper::getVic2ProvinceNumbers(const int EU4ProvinceNumber) const
{
	auto mapping = EU4ToVic2ProvinceMap.find(EU4ProvinceNumber);
	if (mapping != EU4ToVic2ProvinceMap.end())
	{
		return mapping->second;
	}
	else
	{
		std::vector<int> empty;
		return empty;
	}
}

std::vector<int> mappers::ProvinceMapper::getEU4ProvinceNumbers(int Vic2ProvinceNumber) const
{
	auto mapping = Vic2ToEU4ProvinceMap.find(Vic2ProvinceNumber);
	if (mapping != Vic2ToEU4ProvinceMap.end())
	{
		return mapping->second;
	}
	else
	{
		std::vector<int> empty;
		return empty;
	}
}

bool mappers::ProvinceMapper::isProvinceResettable(int Vic2ProvinceNumber, const std::string& region) const
{
	auto regionProvinces = resettableProvinces.find(region);
	if (regionProvinces == resettableProvinces.end())
	{
		return false;
	}

	return (regionProvinces->second.count(Vic2ProvinceNumber) > 0);
}

void mappers::ProvinceMapper::determineValidProvinces()
{
	std::ifstream definitionFile((theConfiguration.getEU4Path() + "/map/definition.csv"));
	if (!definitionFile.is_open())
	{
		LOG(LogLevel::Error) << "Could not open map/definition.csv";
		exit(-1);
	}
	char input[256];
	while (!definitionFile.eof())
	{
		definitionFile.getline(input, 255);
		std::string inputStr(input);
		if (
			(inputStr.substr(0, 8) == "province") ||
			(inputStr.substr(inputStr.find_last_of(';') + 1, 6) == "Unused") ||
			(inputStr.substr(inputStr.find_last_of(';') + 1, 3) == "RNW")
			)
		{
			continue;
		}
		int provNum = std::stoi(inputStr.substr(0, inputStr.find_first_of(';')));
		validProvinces.insert(provNum);
	}
}