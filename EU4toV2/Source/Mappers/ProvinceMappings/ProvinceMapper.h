#ifndef PROVINCE_MAPPER_H
#define PROVINCE_MAPPER_H

#include "ProvinceMappingsVersion.h"
#include "../../EU4World/ColonialRegions/ColonialRegions.h"
#include "../../Configuration.h"
#include "Parser.h"
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
		explicit ProvinceMapper(std::istream& theStream, const Configuration& testConfiguration);

		[[nodiscard]] std::vector<int> getVic2ProvinceNumbers(int eu4ProvinceNumber) const;
		[[nodiscard]] std::vector<int> getEU4ProvinceNumbers(int vic2ProvinceNumber) const;
		[[nodiscard]] bool isProvinceResettable(int vic2ProvinceNumber, const std::string& region) const;
		[[nodiscard]] bool provinceIsInRegion(int province, const std::string& region) const;
		[[nodiscard]] auto isValidProvince(const int province) const { return validProvinces.count(province) > 0; }

	private:
		void registerKeys();
		static ProvinceMappingsVersion getMappingsVersion(const std::map<GameVersion, ProvinceMappingsVersion>& mappingsVersions, const GameVersion& newVersion);
		void createMappings(const ProvinceMappingsVersion& provinceMappingsVersion);
		void addProvincesToResettableRegion(const std::string& regionName, const std::vector<int>& provinces);
		void determineValidProvinces();

		std::map<int, std::vector<int>> vic2ToEU4ProvinceMap;
		std::map<int, std::vector<int>> eu4ToVic2ProvinceMap;
		std::map<std::string, std::set<int>> resettableProvinces;
		std::set<int> validProvinces;
		EU4::ColonialRegions colonialRegionsMapper;
		std::map<GameVersion, ProvinceMappingsVersion> mappingVersions;
	};
}

#endif // PROVINCE_MAPPER_H