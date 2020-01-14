#ifndef PROVINCE_MAPPER_H
#define PROVINCE_MAPPER_H

#include "ProvinceMappingsVersion.h"
#include "../../EU4World/ColonialRegions/ColonialRegions.h"
#include "../../Configuration.h"
#include "newParser.h"
#include <map>
#include <set>

namespace EU4
{
	class Version;
}

namespace mappers
{
	class ProvinceMapper: commonItems::parser
	{
	public:
		ProvinceMapper();
		ProvinceMapper(std::istream& theStream, const Configuration& theConfiguration);

		std::vector<int> getVic2ProvinceNumbers(int EU4ProvinceNumber) const;
		std::vector<int> getEU4ProvinceNumbers(int Vic2ProvinceNumber) const;
		bool isProvinceResettable(int Vic2ProvinceNumber, const std::string& region) const;

		bool isValidProvince(int province) const { return validProvinces.count(province) > 0; }
		bool provinceIsInRegion(int province, const std::string& region) const;

	private:
		void registerKeys();
		ProvinceMappingsVersion getMappingsVersion(const std::map<EU4::Version, ProvinceMappingsVersion>& mappingsVersions, const Configuration& theConfiguration);
		void createMappings(const ProvinceMappingsVersion& provinceMappingsVersion);
		void addProvincesToResettableRegion(const std::string& regionName, const std::vector<int>& provinces);
		void determineValidProvinces();

		std::map<int, std::vector<int>> Vic2ToEU4ProvinceMap;
		std::map<int, std::vector<int>> EU4ToVic2ProvinceMap;
		std::map<std::string, std::set<int>> resettableProvinces;
		std::set<int> validProvinces;
		EU4::ColonialRegions colonialRegionsMapper;
		std::map<EU4::Version, ProvinceMappingsVersion> mappingVersions;
	};
}

#endif // PROVINCE_MAPPER_H