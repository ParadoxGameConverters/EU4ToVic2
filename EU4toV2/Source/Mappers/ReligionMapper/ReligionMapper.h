#ifndef RELIGION_MAPPER_H
#define RELIGION_MAPPER_H

#include "newParser.h"
#include <map>
#include <optional>
#include <string>

namespace mappers
{
	class ReligionMapper: commonItems::parser
	{
	public:
		ReligionMapper();
		ReligionMapper(std::istream& theStream);
		std::optional<std::string> getVic2Religion(const std::string& EU4Religion) const;

	private:
		void registerKeys();
		std::map<std::string, std::string> EU4ToVic2ReligionMap;
	};
}

#endif // RELIGION_MAPPER_H
