#ifndef CULTURE_MAPPING_RULE_H
#define CULTURE_MAPPING_RULE_H

#include "newParser.h"
#include <set>

namespace EU4 {
	class Regions;
}

namespace mappers
{
	class CultureMappingRule: commonItems::parser
	{
	public:
		CultureMappingRule() = default;
		explicit CultureMappingRule(std::istream& theStream);
		
		[[nodiscard]] std::optional<std::string> cultureMatch(
			const EU4::Regions& eu4Regions,
			const std::string& eu4culture,
			const std::string& eu4religion,
			int eu4Province,
			const std::string& eu4ownerTag) const;

	private:
		std::string destinationCulture;
		std::set<std::string> cultures;
		std::set<std::string> religions;
		std::set<std::string> regions;
		std::set<std::string> owners;
		std::set<int> provinces;
	};
}

#endif // CULTURE_MAPPING_RULE_H
