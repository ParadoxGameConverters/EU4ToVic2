#ifndef EU4_ID_H
#define EU4_ID_H

#include "newParser.h"

namespace EU4
{
	class ID: commonItems::parser
	{
	public:
		ID() = default;
		explicit ID(std::istream& theStream);

		[[nodiscard]] auto getIDNum() const { return IDNum; }

	private:
		int IDNum = 0;
		int IDType = 0;
	};
}

#endif // EU4_ID_H