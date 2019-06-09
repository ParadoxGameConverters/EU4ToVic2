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



#include "ProvinceMapper.h"
#include "ProvinceMappingsVersion.h"
#include "../../Configuration.h"
#include "../../EU4World/EU4Version.h"
#include "Log.h"
#include <stdexcept>



mappers::ProvinceMapper::ProvinceMapper(std::istream& theStream, const Configuration& configuration)
{
	std::map<EU4::Version, ProvinceMappingsVersion> mappingVersions;
	registerKeyword(std::regex("[0-9\\.]+"), [&mappingVersions](const std::string& versionString, std::istream& theStream){
		ProvinceMappingsVersion version(versionString, theStream);
		mappingVersions.insert(std::make_pair(version.getVersion(), version));
	});

	parseStream(theStream);

	ProvinceMappingsVersion mappings = getMappingsVersion(mappingVersions, configuration);
	createMappings(mappings);
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