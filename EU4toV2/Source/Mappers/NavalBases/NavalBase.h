#ifndef NAVAL_BASE_H
#define NAVAL_BASE_H

#include "newParser.h"

namespace mappers
{
	class NavalBase: commonItems::parser
	{
	public:
		NavalBase() = default;
		NavalBase(std::istream& theStream);
		bool getFound() const { return found; }

	private:
		bool found = false;
	};
}

#endif // NAVAL_BASE_H
