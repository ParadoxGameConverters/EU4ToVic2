#ifndef POP_MAPPER_H
#define POP_MAPPER_H

#include "newParser.h"
#include "PopTypes.h"
#include <map>

namespace mappers
{
	class PopMapper : commonItems::parser
	{
	public:
		PopMapper() = default;
		explicit PopMapper(std::istream& theStream);
		explicit PopMapper(const std::string& filename);
		
		[[nodiscard]] const auto& getProvincePopTypeMap() const { return provincePopTypeMap; }

	private:
		void registerKeys();
		std::map<int, PopTypes> provincePopTypeMap;
	};
}

#endif // POP_MAPPER_H
