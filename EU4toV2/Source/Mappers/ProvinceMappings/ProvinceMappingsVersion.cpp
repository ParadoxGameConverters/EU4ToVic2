#include "ProvinceMappingsVersion.h"
#include "ParserHelpers.h"

mappers::ProvinceMappingsVersion::ProvinceMappingsVersion(const std::string& versionString, std::istream& theStream)
{
	registerKeyword("link", [this](const std::string& unused, std::istream& theStream)
		{
			const ProvinceMapping newMapping(theStream);
			mappings.push_back(newMapping);
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
	
	parseStream(theStream);
	clearRegisteredKeywords();

	version = EU4::Version(versionString);
}