#ifndef POP_TYPE_H
#define POP_TYPE_H

#include "newParser.h"
#include "Pop.h"
#include <map>

namespace mappers
{
	class PopType : commonItems::parser
	{
	public:
		PopType() = default;
		PopType(std::istream& theStream);
		std::map<std::string, Pop> getPopTypes() const { return popTypes; }

	private:
		std::map<std::string, Pop> popTypes;
	};
}

#endif // POP_TYPE_H
