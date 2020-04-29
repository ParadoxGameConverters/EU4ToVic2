#ifndef PROVINCE_MAPPING_VERSION_H
#define PROVINCE_MAPPING_VERSION_H

#include "Parser.h"
#include "ProvinceMapping.h"
#include "../../EU4World/EU4Version.h"

namespace mappers
{
	class ProvinceMappingsVersion: commonItems::parser
	{
	public:
		ProvinceMappingsVersion(const std::string& versionString, std::istream& theStream);

		[[nodiscard]] const auto& getVersion() const { return version; }
		[[nodiscard]] const auto& getMappings() const { return mappings; }

	private:
		EU4::Version version;
		std::vector<ProvinceMapping> mappings;
	};
}

#endif // PROVINCE_MAPPING_VERSION_H