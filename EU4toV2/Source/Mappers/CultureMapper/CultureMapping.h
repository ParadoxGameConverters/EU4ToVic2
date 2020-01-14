#ifndef CULTURE_MAPPING_H
#define CULTURE_MAPPING_H

#include "../../EU4World/Regions/Regions.h"
#include <map>
#include <optional>
#include <string>

namespace mappers
{
	enum class DISTINGUISHER_TYPES
	{
		OWNER,
		RELIGION,
		PROVINCE,
		REGION
	};

	class CultureMapping
	{
	public:
		CultureMapping(
			const std::string& sourceCulture,
			const std::string& destinationCulture,
			const std::map<DISTINGUISHER_TYPES, std::string>& distinguishers
		);

		std::optional<std::string> cultureMatch(
			const EU4::Regions& EU4Regions,
			const std::string& culture,
			const std::string& religion,
			int EU4Province,
			const std::string& ownerTag
		) const;

	private:
		bool distinguishersMatch(
			const EU4::Regions& EU4Regions,
			const std::map<DISTINGUISHER_TYPES, std::string>& distinguishers,
			const std::string& religion,
			int EU4Province,
			const std::string& ownerTag
		) const;

		std::string sourceCulture;
		std::string destinationCulture;
		std::map<DISTINGUISHER_TYPES, std::string> distinguishers;
	};
}

#endif // CULTURE_MAPPING_H