#ifndef CULTURAL_UNION_MAPPER_H
#define CULTURAL_UNION_MAPPER_H

#include "newParser.h"

namespace mappers
{
	class CulturalUnionMapper: commonItems::parser
	{
	public:
		CulturalUnionMapper() = default;
		explicit CulturalUnionMapper(std::istream& theStream);
		
		void loadFile(const std::string& filename);

		[[nodiscard]] std::optional<std::vector<std::string>> getCoresForCulture(const std::string& culture) const;

	private:
		void registerKeys();
		std::map<std::string, std::vector<std::string>> unionMap;
	};
}

#endif //CULTURAL_UNION_MAPPER_H