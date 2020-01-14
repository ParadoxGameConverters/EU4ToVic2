#ifndef POP_TYPES_H
#define POP_TYPES_H

#include "newParser.h"
#include "Pop.h"
#include <map>

namespace mappers
{
	class PopTypes : commonItems::parser
	{
	public:
		PopTypes() = default;
		PopTypes(std::istream& theStream);
		const std::multimap<std::string, Pop>& getPopTypes() const { return popTypes; }

	private:
		std::multimap<std::string, Pop> popTypes;
	};
}

#endif // POP_TYPES_H
