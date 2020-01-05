#ifndef POP_MAPPER_H
#define POP_MAPPER_H

#include "newParser.h"
#include "PopType.h"
#include <map>

namespace mappers
{
	class PopMapper : commonItems::parser
	{
	public:
		PopMapper() = default;
		PopMapper(std::istream& theStream);
		std::map<int, PopType> getPopMap() const { return popMap; }

	private:
		std::map<int, PopType> popMap;
	};
}

#endif // POP_MAPPER_H
