#ifndef CULTURE_MAPPER_H
#define CULTURE_MAPPER_H

#include "newParser.h"
#include "../../EU4World/Regions/Regions.h"
#include <optional>
#include <string>
#include <vector>
#include "CultureMappingRule.h"

namespace mappers
{
	class CultureMapper: commonItems::parser
	{
	public:
		CultureMapper() = default;
		CultureMapper(std::istream& theStream);
		void loadFile(const std::string& fileName);

		std::optional<std::string> cultureMatch(
			const EU4::Regions& EU4Regions,
			const std::string& EU4culture,
			const std::string& EU4religion,
			int EU4Province,
			const std::string& EU4ownerTag
		) const;

	private:
		void registerKeys();
		std::vector<CultureMappingRule> cultureMapRules;
	};
}

#endif // CULTURE_MAPPER_H