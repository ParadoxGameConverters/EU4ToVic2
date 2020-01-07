#ifndef EU4EMPIRE_H
#define EU4EMPIRE_H

#include "newParser.h"

namespace EU4
{
	class EU4Empire :  commonItems::parser
	{
	public:
		EU4Empire(std::istream& theStream);
		std::string getEmperor() const { return emperor; }

	private:
		std::string emperor;
	};
}

#endif // EU4EMPIRE_H