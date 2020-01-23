#ifndef EU4WAR_DETAILS_H
#define EU4WAR_DETAILS_H

#include "Date.h"
#include "newParser.h"

namespace EU4
{
	class WarDetails :  commonItems::parser
	{
	public:
		WarDetails(std::istream& theStream);

	private:
	};
}

#endif // EU4WAR_DETAILS_H