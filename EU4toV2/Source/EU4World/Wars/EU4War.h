#ifndef EU4WAR_H
#define EU4WAR_H

#include "Date.h"
#include "newParser.h"

namespace EU4
{
	class War : commonItems::parser
	{
	public:
		War(std::istream& theStream);

	private:
		std::string name;
		date startDate;
		std::vector<std::string> attackers;
		std::vector<std::string> defenders;
		std::string casusBelli;
		int targetProvince = 0;
		std::string warType;
	};
}

#endif // EU4WAR_H