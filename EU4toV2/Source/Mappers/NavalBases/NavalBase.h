#ifndef NAVAL_BASE_H
#define NAVAL_BASE_H

#include "newParser.h"

namespace mappers
{
	class NavalBase: commonItems::parser
	{
	public:
		NavalBase() = default;
		explicit NavalBase(std::istream& theStream);
		
		[[nodiscard]] auto getFound() const { return found; }

	private:
		bool found = false;
	};
}

#endif // NAVAL_BASE_H
