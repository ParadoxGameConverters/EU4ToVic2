#ifndef CULTURE_MAPPER_H
#define CULTURE_MAPPER_H

#include "newParser.h"
#include "CultureMapping.h"
#include "../../EU4World/Regions/Regions.h"
#include <optional>
#include <string>
#include <vector>

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
			const std::string& culture,
			const std::string& religion,
			int EU4Province,
			const std::string& ownerTag
		) const;

	private:
		void registerKeys();
		std::vector<CultureMapping> cultureMap;
	};
}

#endif // CULTURE_MAPPER_H