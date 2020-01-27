#ifndef POP_TYPES_H
#define POP_TYPES_H

#include "newParser.h"
#include "MapperPop.h"
#include <map>

namespace mappers
{
	class PopTypes : commonItems::parser
	{
	public:
		PopTypes() = default;
		explicit PopTypes(std::istream& theStream);
		
		[[nodiscard]] const auto& getPopTypes() const { return popTypes; }

	private:
		std::multimap<std::string, MapperPop> popTypes;
	};
}

#endif // POP_TYPES_H
