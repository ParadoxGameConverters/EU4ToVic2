#include "ProvinceMappingsVersion.h"

mappers::ProvinceMappingsVersion::ProvinceMappingsVersion(const std::string& versionString, std::istream& theStream)
{
	registerKeyword("link", [this](const std::string& unused, std::istream& theStream)
		{
			ProvinceMapping newMapping(theStream);
			mappings.push_back(newMapping);
		});
	parseStream(theStream);

	version = EU4::Version(versionString);
}