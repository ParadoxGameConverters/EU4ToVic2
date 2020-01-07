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
		PopMapper(std::istream& theStream);
		const std::map<int, PopTypes>& getProvincePopTypeMap() const { return provincePopTypeMap; }

	private:
		std::map<int, PopTypes> provincePopTypeMap;
	};
}

#endif // POP_MAPPER_H
