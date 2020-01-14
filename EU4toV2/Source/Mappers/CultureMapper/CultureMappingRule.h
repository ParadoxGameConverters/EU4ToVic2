#ifndef CULTURE_MAPPING_RULE_H
#define CULTURE_MAPPING_RULE_H

#include "newParser.h"
#include "CultureMapping.h"
#include <vector>

namespace mappers
{
	class CultureMappingRule: commonItems::parser
	{
	public:
		CultureMappingRule(std::istream& theStream);
		std::vector<CultureMapping> getMappings() const { return mappings; }

	private:
		std::vector<CultureMapping> mappings;
		std::string destinationCulture;
		std::vector<std::string> sourceCultures;
		std::map<mappers::DISTINGUISHER_TYPES, std::string> distinguishers;
	};
}

#endif // CULTURE_MAPPING_RULE_H
