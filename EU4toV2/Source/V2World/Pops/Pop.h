#ifndef POP_H
#define POP_H

#include "newParser.h"

namespace mappers
{
	class Pop: commonItems::parser
	{
	public:
		Pop() = default;
		Pop(std::istream& theStream);
		std::string getCulture() const { return culture; }
		std::string getReligion() const { return religion; }
		int getSize() const { return size; }

	private:
		std::string culture;
		std::string religion;
		int size = 0;
	};
}

#endif // POP_H
